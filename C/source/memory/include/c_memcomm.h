/**
 * @file   c_memcomm.h
 * @author jesson3264@163.com
 * @date   2021-01-01
 * @brief  share memory and normal memory common info.
 * */
#ifndef MEMORY_MEMCOMM_H
#define MEMORY_MEMCOMM_H
#include <stdint.h>
#include <stddef.h>

#pragma pack(1)
typedef struct ChunkHead
{
    //uint32_t reserve_field;  ///<
    uint32_t size;             ///< user use size.
    char data[0];
}ChunkHead;
#pragma pack()

enum { ALIGN = 8 };

typedef union obj {
    union obj* free_list_link;
    char client_data[1];
}obj;

/**
 * @brief ROUND_UP by ALIGN
 *
**/
size_t ROUND_UP(size_t bytes);

#endif
