/**
 * @file scl_miniunit.h
 * @details MinUnit -- a minimal unit testing framework for C
 */
#ifndef SCL_COMMON_MINIUNIT_H
#define SCL_COMMON_MINIUNIT_H

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
extern int tests_run;

#endif