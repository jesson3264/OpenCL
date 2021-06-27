
#ifndef C_COMMON_STRINGS_CPYSTRN_H
#define C_COMMON_STRINGS_CPYSTRN_H
#include <stddef.h>
/** use apache apr apr_cpystrn() function codes.*/
/*
 * Apache's "replacement" for the strncpy() function. We roll our
 * own to implement these specific changes:
 *   (1) strncpy() doesn't always null terminate and we want it to.
 *   (2) strncpy() null fills, which is bogus, esp. when copy 8byte
 *       strings into 8k blocks.
 *   (3) Instead of returning the pointer to the beginning of
 *       the destination string, we return a pointer to the
 *       terminating '\0' to allow us to "check" for truncation
 *   (4) If src is NULL, null terminate dst (empty string copy)
 *
 * apr_cpystrn() follows the same call structure as strncpy().
 */
char * scl_cpystrn(char *dst, const char *src, size_t dst_size);
#endif