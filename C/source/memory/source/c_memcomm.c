#include "c_memcomm.h"

/**
 * @brief ALIGN 对齐
 * 
 * @param bytes 
 * @return size_t 
 */
size_t ROUND_UP(size_t bytes) 
{
    return (((bytes) + ALIGN - 1) & ~(ALIGN - 1));
}