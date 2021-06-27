#include "scl_miniunit.h"
#include "testlink.h"
#include "testqueue.h"
#include "teststack.h"
#include "testcmap.h"
#include "testcmap.h"
#include "test-mempool.h"
#include "test-skiplist.h"

#include <stdio.h>
int tests_run = 0;
int main(int argc, char *argv)
{
	//testslink();
	//testdlink();
	printf("Hello World.\n");
	//testqueue();
	//testdqueue2();
	//teststack();
	printf("\n====start test map.===============\n");
	//testmap();
	printf("\n==== start test skiplist.===============\n");
	//test_skiplist();
	printf("\n==== end   test skiplist.===============\n");
	printf("\n");

	fprintf(stderr, "\n====start test_mempool.===============\n");
	test_mempool();
	fprintf(stderr, "\n====end   test_mempool.===============\n");
	fprintf(stderr, "\n====start test-random.===============\n");
	gen_random();
	fprintf(stderr, "\n====end  test- random.===============\n");
	return 0;
}

/*

*/