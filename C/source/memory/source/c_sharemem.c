#include "c_sharemem.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

void* ShareRefill(CShareMemPool* , size_t n);

char* ShareChunkAlloc(CShareMemPool* mp, size_t size, int* nobjs);

static size_t SHARE_FREELIST_INDEX(size_t bytes)
{
    return (((bytes) + ALIGN - 1)/ALIGN - 1);
}

CShareMemPool* ShareCreateMempool(size_t memsize, ErrorCallback call_back)
{
    int user_size = ROUND_UP(memsize);

    int total_size = ROUND_UP(user_size + sizeof(ChunkHead)) + ROUND_UP(sizeof(CShareMemPool));
    void* sharemem =  mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED | MAP_LOCKED, -1, 0);
    if (sharemem == (void*)-1)
    {
        fprintf(stderr, "mmap share memory failured. errno:%d strerror:%s\n", errno, strerror(errno));
        return NULL;
    }

    memset(sharemem, 0, total_size);
    char* mem_start = (char*)sharemem;
    
    CShareMemPool* smp = (CShareMemPool*)sharemem;
    smp->share_mem_start = mem_start;
    smp->start_free = mem_start + ROUND_UP(sizeof(CShareMemPool)); // ROUND_UP for align
    smp->end_free = mem_start + total_size;
    smp->total_size = total_size;
    smp->call_back = call_back;
    memset(smp->free_list, 0, sizeof(obj)*SHARE_NFREELISTS);

    return smp;
}

void* ShareAllocate(CShareMemPool* smp, size_t n)
{
    size_t srcn = n;
    obj** my_free_list;
    obj* result;
    ChunkHead* head = NULL;
    if (n > (size_t)SHARE_MAX_BYTES)
    {
        //TODO
        return NULL;
    }

    n += sizeof(ChunkHead);
    n = ROUND_UP(n);

    obj** free_list = smp->free_list;
    int idx = SHARE_FREELIST_INDEX(n);
    my_free_list = free_list + idx;
    result = *my_free_list;
    if (result == 0)
    {
        void* r = ShareRefill(smp, ROUND_UP(n));
        if (!r)
        {
            fprintf(stderr, "no space for size: %d\n", srcn);
            return NULL;
        }

        head = (ChunkHead*)r;
        head->size = n;
        return head->data;
    }

    *my_free_list = result->free_list_link;
    head = (ChunkHead*)result;
    head->size = n;
    return head->data;
}

/**
 * @brief 
 */
void ShareDeAllocate(CShareMemPool* smp, void* p)
{
    char* cp = (char*)p;
    cp -= sizeof(ChunkHead);
    ChunkHead* head = (ChunkHead*)cp;
    size_t s = head->size;
    obj ** my_free_list = smp->free_list + SHARE_FREELIST_INDEX(s);
    obj *q = (obj*)cp;
    q->free_list_link = *my_free_list;
    *my_free_list = q;
}

/**
 * @brief 
 */
void ShareDestroyMempool(CShareMemPool* smp)
{
    if (munmap(smp->share_mem_start, smp->total_size) == -1)
    {
        fprintf(stderr, "munmap failured. errno:%d strerror:%s", errno, strerror(errno));
    }
}

/**
 * @brief 
 */
void* ShareRefill(CShareMemPool* mp, size_t n)
{
    int nobjs = 20;
    char* chunk = ShareChunkAlloc(mp, n, &nobjs);
    if (!chunk)
    {
        if (mp->call_back)
        {
            mp->call_back(mp, __FILE__, __LINE__, SHARE_MEM_ERROR__NO_SPACE);
        }

        fprintf(stderr, "no space for size:%d\n", n);
        return NULL;
    }

    obj** my_free_list;
    obj* result;
    obj* current_obj;
    obj* next_obj;
    int i = 0;
    if (1 == nobjs) 
        return (chunk);
        
    my_free_list = mp->free_list + SHARE_FREELIST_INDEX(n);

    result = (obj*)chunk;
    // 存储的是分配的第二个节点
    *my_free_list = next_obj = (obj*)(chunk + n);
    for (i = 1; ; i++)
    {
        current_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        if (nobjs - 1 == i)
        {
            current_obj->free_list_link = 0;
            break;
        }
        else 
        {
            current_obj->free_list_link = next_obj;
        }
    }

    return result;
}

/**
 * @brief
 * @details distribute nobjs of size size
 **/
char* ShareChunkAlloc(CShareMemPool* mp, size_t size, int* nobjs)
{
    if (!mp || !nobjs)
    {
        return NULL;
    }

    char* result;
    size_t total_bytes = size * (*nobjs);
    size_t bytes_left = mp->end_free - mp->start_free;

    if (bytes_left >= total_bytes)
    {
        result = mp->start_free;
        mp->start_free += total_bytes;
        return result;
    }
    else if (bytes_left >= size)
    {
        *nobjs = bytes_left/size;
        total_bytes = size * (*nobjs);
        result = mp->start_free;
        mp->start_free += total_bytes;
        return result;
    }
    else 
    {
        *nobjs = 0;
        ///< recycle span space to freelist of index SHARE_FREELIST_INDEX(bytes_left)
        if (bytes_left > 0)
        {
            obj** my_free_list = mp->free_list + SHARE_FREELIST_INDEX(bytes_left);
            ((obj*)mp->start_free)->free_list_link = * my_free_list;
            *my_free_list = (obj*)mp->start_free;
        }
    }

    return NULL;
}

void ShowShareMempoolInfo(CShareMemPool* smp)
{
    fprintf(stderr, "\n=========== Start CShareMemPool Info. ============\n");
    fprintf(stderr, "sizeof CShareMemPoll :%x\n", sizeof(CShareMemPool));
    fprintf(stderr, "share_mem_start:%p\n", smp->share_mem_start);
    fprintf(stderr, "total_size:%d\n", smp->total_size);
    
    fprintf(stderr, "start_free:%p\n", smp->start_free);
    fprintf(stderr, "end_free:%p\n", smp->end_free);
    fprintf(stderr, "=========== End   CShareMemPool Info. ============\n\n");
}