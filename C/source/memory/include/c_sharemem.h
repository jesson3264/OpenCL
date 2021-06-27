/**
 * @file      c_sharemem.h
 * @author    jesson3264@163.com
 * @brief     2021-01-02
 * @details
 * @interface
 **/
#ifndef MEMORY_C_SHAREMEM_H
#define MEMORY_C_SHAREMEM_H
#include "c_memcomm.h"

enum { SHARE_MAX_BYTES = 4096 };
enum { SHARE_NFREELISTS = SHARE_MAX_BYTES / ALIGN};

#define SHARE_MEM_ERROR__PARAMETER   1
#define SHARE_MEM_ERROR__MMAP_FAIL   2
#define SHARE_MEM_ERROR__NO_MEMORY   3  ///< no memory
#define SHARE_MEM_ERROR__NO_SPACE    4  ///< init space is used.


struct CShareMemPool;

typedef int (*ErrorCallback)(struct CShareMemPool* smp, const char* filename, int line, int errorno);

typedef struct CShareMemPool
{
    char* share_mem_start;
    ErrorCallback call_back;   ///< if occour error, it will be call.
    size_t  total_size;
    union obj* free_list[SHARE_NFREELISTS];
    char* start_free;
    char* end_free;
}CShareMemPool;

/**
 * @brief if occour error, then revoke call_back
**/
CShareMemPool* ShareCreateMempool(size_t memsize, ErrorCallback call_back);

void ShareDestroyMempool(CShareMemPool* );

void* ShareAllocate(CShareMemPool* , size_t n);

void ShareDeAllocate(CShareMemPool* , void* p);

/**
 * for debug.
 */
void ShowShareMempoolInfo(CShareMemPool*);
#endif
