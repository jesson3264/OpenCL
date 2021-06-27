#ifndef MEMORY_C_MEMPOOL_H
#define MEMORY_C_MEMPOOL_H
#include <stdint.h>
#include <stddef.h>
#include "scl_double_list.h"
#include "c_memcomm.h"
/** 后续可使用内存创建函数 */
#define MALLOC  malloc
#define FREE    free


enum { MAX_BYTES = 128 };
enum { NFREELISTS = MAX_BYTES/ALIGN };

/**
 * @brief 存储已经分配的块内存链表
 * 
 */
typedef struct AllocatedList
{
    d_node_t list;
    char* addr;
}AllocatedList;

typedef struct CMemPool
{
    union obj* free_list[NFREELISTS];
    char* start_free;
    char* end_free;
    size_t heap_size;

    /** 记录分配的内存块 */ 
    AllocatedList alloc_list;
}CMemPool;

/**
 * @brief Create a Mempool object
 * 
 * @param memsize 
 * @return CMemPool* 
 */
CMemPool* CreateMempool(size_t memsize);

/**
 * @brief 
 * 
 * @param mp 
 */
void DestroyMempool(CMemPool* mp);

/**
 * @brief 
 * @return SCL_SUCCESS success, other failured.
 * 
 */
void* Allocate(CMemPool* mp, size_t n);

/**

TODO: 需再 要分配的n 前面加上一个头部，返回的时候，只返回头部后面的数据 
*/
void DeAllocate(CMemPool* mp, void* p, size_t n);

/** FOR DEBUG. */ 
void ShowMempoolInfo(CMemPool* mp);


void* refill(CMemPool* mp, size_t n);

char* ChunkAlloc(CMemPool* mp, size_t size, int* nobjs);

#endif
