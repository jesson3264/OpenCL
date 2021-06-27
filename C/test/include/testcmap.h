#ifndef __TEST_CMAP_H__
#define __TEST_CMAP_H__

int key_cmp_int(void* key1, void* key2);

int value_cmp_int(void* key1, void* key2);

int hash_func_int(void *key);
void testmap();
#endif