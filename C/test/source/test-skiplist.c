#include "test-skiplist.h"
#include "skiplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define COUNT	10
int test_skiplist()
{

    skip_list_t sl;
	int i, tmp, *keys;

	keys = (int*)malloc(COUNT*sizeof(int));

	srand(time(NULL));

	sl = skip_list_create();

	for(i=0; i<COUNT; i++) {
		keys[i] = rand();
		tmp = rand();
		printf("put %5d : %5d, return %5d", keys[i], tmp, (int)skip_list_put(sl, keys[i], (void*)tmp));
		printf(", count=%d\n", skip_list_count(sl));
	}

	puts("*****************************************");

	for(i=0; i<COUNT; i++) {
		printf("put %5d : %5d, return %d\n", keys[i], keys[i], (int)skip_list_put(sl, keys[i], (void*)keys[i]));
	}

	puts("*****************************************");

	skip_list_key_set(sl, keys, COUNT);
	printf("key set : ");
	for(i=0; i<COUNT-1; i++) {
		printf("%d, ", keys[i]);
	}
	printf("%d\n", keys[COUNT-1]);

	puts("*****************************************");

	for(i=0; i<COUNT; i++) {
		printf("get %5d, return %d\n", keys[i], (int)skip_list_get(sl, keys[i]));
	}

	puts("*****************************************");

	for(i=0; i<COUNT; i++) {
		printf("constains %5d, return %d\n", keys[i], skip_list_contains(sl, keys[i]));
	}

	puts("*****************************************");

	for(i=0; i<COUNT; i++) {
		printf("remove %5d, return %5d", keys[i], (int)skip_list_remove(sl, keys[i]));
		printf(", count=%d\n", skip_list_count(sl));
	}

	puts("*****************************************");

	for(i=0; i<COUNT; i++) {
		printf("constains %5d, %d\n", keys[i], skip_list_contains(sl, keys[i]));
	}

	skip_list_destroy(sl);

	free(keys);

	return 0;
}