#include "c_mempool.h"
#include "core_errno.h"
#include "scl_double_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static size_t FREELIST_INDEX(size_t bytes)
{
    return (((bytes) + ALIGN - 1)/ALIGN - 1);
}

CMemPool* CreateMempool(size_t memsize)
{
    if (memsize <= 0)
    {
        return NULL;
    }
    
    int user_size = ROUND_UP(memsize);
    /** 至少包含一个数据头部 */
    int total_size = (user_size + sizeof(CMemPool) + sizeof(ChunkHead));
    char* mem = MALLOC(total_size);
    if (!mem)
    {
        return NULL;
    }

    memset(mem, 0, total_size);
    CMemPool* mp = (CMemPool*)mem;

    mp->start_free = 0;
    mp->end_free = 0;
    mp->heap_size = 0;
    memset(mp->free_list, 0, sizeof(obj)*NFREELISTS);

    dlist_init(&(mp->alloc_list.list));
    mp->alloc_list.addr = NULL;

    return mp;
}
// TODO
void DestroyMempool(CMemPool* mp)
{
    return ;
}

void* Allocate(CMemPool* mp, size_t n)
{
    obj** my_free_list;
    obj* result;
    ChunkHead* head = NULL;
    if (n > (size_t)MAX_BYTES)
    {
        //TODO
        return NULL;
    }

    // 添加一个数据头部便于数据归还
    n += sizeof(ChunkHead);
    n = ROUND_UP(n);

    obj** free_list = mp->free_list;

    my_free_list = free_list + FREELIST_INDEX(n);
    result = *my_free_list;
    if (result == 0)
    {
        void* r = refill(mp, ROUND_UP(n));
        head = (ChunkHead*)r;
        head->size = n;///< 包含头部
        return head->data;
    }

    // 指向下一个元素
    *my_free_list = result->free_list_link;
    head = (ChunkHead*)result;
    head->size = n;
    return head->data;
}

void DeAllocateP(CMemPool* mp, void*p)
{
    return ;
}

void DeAllocate(CMemPool* mp, void* p, size_t n)
{
    obj* q = (obj*)p;
    obj** my_free_list;
    if (n > (size_t)MAX_BYTES)
    {
        //TODO
        return ;
    }

    my_free_list = mp->free_list + FREELIST_INDEX(n);
    q->free_list_link = *my_free_list;
    *my_free_list = q;
}

void* refill(CMemPool* mp, size_t n)
{
    int nobjs = 20;
    char* chunk = ChunkAlloc(mp, n, &nobjs);
    obj** my_free_list;
    obj* result;
    obj* current_obj;
    obj* next_obj;
    int i = 0;
    if (1 == nobjs) return (chunk);
    my_free_list = mp->free_list + FREELIST_INDEX(n);

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

char* ChunkAlloc(CMemPool* mp, size_t size, int* nobjs)
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
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(mp->heap_size >> 4);
        ///< 回收空闲池中的碎片 
        if (bytes_left > 0)
        {
            // 走到这里  bytes_left 不会大于 128 就是 NFREELISTS * 16
            obj** my_free_list = mp->free_list + FREELIST_INDEX(bytes_left);
            ((obj*)mp->start_free)->free_list_link = * my_free_list;
            *my_free_list = (obj*)mp->start_free;
        }

        mp->start_free = (char*)MALLOC(bytes_to_get);
        

        if (0 == mp->start_free)
        {
            fprintf(stderr, "call MALLOC failured.%d %s", errno, strerror(errno));
            return NULL;
        }
        
        AllocatedList *node = MALLOC(sizeof(AllocatedList));
        node->addr = mp->start_free;
        dlist_init(&(node->list));
        dlist_add_tail(&mp->alloc_list.list, &node->list);


        mp->heap_size += bytes_to_get;
        mp->end_free = mp->start_free + bytes_to_get;

        // 申请号了内存，重新进行分配。
        return (ChunkAlloc(mp, size, nobjs));
    }

    // un-reach.
    return NULL;
}
