#ifndef CORE_COMMON_H
#define CORE_COMMON_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Type for specifying an error or status code.
 */
typedef int scl_status_t;
char* scl_strerror(scl_status_t statcode, char *buf, size_t bufsize);

/**
 * SCL_OS_START_ERROR is where the SCL specific error values start.
 */
#define SCL_OS_START_ERROR     20000

/**
 * SCL_OS_ERRSPACE_SIZE is the maximum number of errors you can fit
 *    into one of the error/status ranges below -- except for
 *    SCL_OS_START_ERROR, which see.
 */
#define SCL_OS_ERRSPACE_SIZE 50000

/**
 * SCL_OS_START_STATUS is where the APR specific status codes start.
 */
#define SCL_OS_START_STATUS    (SCL_OS_START_ERROR + SCL_OS_ERRSPACE_SIZE)

#define SCL_OS_START_USERERR    (SCL_OS_START_STATUS + SCL_OS_ERRSPACE_SIZE)

/** no error. */
#define SCL_SUCCESS 0


#define SCL_FIRST_ERROR (SCL_OS_START_ERROR + 1)

#define SCL_PARAM_ERROR   (SCL_FIRST_ERROR + 1)

/** 剩余空间不足 */
#define SCL_MEMPOOL_LACK_OF_REST   (SCL_FIRST_ERROR + 1000)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif