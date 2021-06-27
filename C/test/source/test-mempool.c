/**
 * @file      test-mempool.c
 * @author    jesson3264@163.com
 * @brief     2021-01-02
 * @details
 * @interface
 **/
#include "test-mempool.h"
#include "scl_miniunit.h"
#include "c_mempool.h"
#include "c_sharemem.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

void *p = NULL;
void *p2 = NULL;
void *p3 = NULL;

static char * test_p() 
{
    mu_assert("p != NULL", p == NULL);
    return 0;
}

static char * test_p2() 
{
    mu_assert("p2 != NULL", p2 == NULL);
    return 0;
}
 
static char * test_p3() 
{
    mu_assert("p3 != NULL", p3 == NULL);
    return 0;
}

static char * all_tests() 
{
    mu_run_test(test_p);
    //mu_run_test(test_p2);
	//mu_run_test(test_p3);
    return 0;
}

int TestErrorCallback(struct CShareMemPool* smp, const char* filename, int line, int errorno)
{
	ShowShareMempoolInfo(smp);
	fprintf(stderr, "filename:%s line:%d errorno:%d\n", filename, line, errorno);
	return 0;
}

void test_mempool()
{
	int i = 0;
	CShareMemPool* smp = ShareCreateMempool(256, TestErrorCallback);
	ShowShareMempoolInfo(smp);

	for (i = 0; ; ++i)
	{
		p = ShareAllocate(smp, 8);
		fprintf(stderr, "i:%d\n", i);
		if (!p)
			break;
	}

	fprintf(stderr, "total=1024, i:%d\n", i);


	char *result = all_tests();
    if (result != 0) 
	{
        fprintf(stderr, "%s\n", result);
    }
    else 
	{
        fprintf(stderr, "ALL TESTS PASSED\n");
    }

    fprintf(stderr, "Tests run: %d\n", tests_run);

	return ;
}