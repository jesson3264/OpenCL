#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

void* func_create_key_int(void* key)
{
    int* k = (int*)key;
    int* t = (void*)malloc(sizeof(*k));
    memcpy(t, k, sizeof(*k));
    return (void*)t;
}

void func_erase_key_int(void* key)
{
    free(key);
}

void* func_create_val_int(void* key)
{
    int* k = (int*)key;
    int* t = (void*)malloc(sizeof(*k));
    memcpy(t, k, sizeof(*k));
    return (void*)t;
}

void func_erase_val_int(void* key)
{
    free(key);
}

int func_key_cmp_int(void* key1, void* key2)
{
    return memcmp(key1, key2, sizeof(int));
}

size_t hash_int(void* key)
{
    return (size_t)(*((int*)key));
}


size_t func_hash_key_str(void* key)
{
    return 0;
}

int func_key_cmp_str(void* key1, void* key2)
{
    const unsigned char* str1 = (unsigned char*)key1;
    const unsigned char* str2 = (unsigned char*)key2;
    return strcmp(str1, str2);
}

void test_read_ini()
{
    printf("start read ini.");
    
    chash* section = chash_create(13, 
        func_create_key_str, 
        func_erase_val_str, 
        func_create_val_str, 
        func_erase_val_str,
        func_hash_key_str, 
        func_key_cmp_str);

    return;
}

int main(int argc, char** argv)
{
    int i = 0;
    int** arr = (int**)calloc(sizeof(int*), 3);
    //memset(arr, 0, sizeof(int*) * 3);


    int** p = arr;

    printf("p:%p  ar:%p\n", p, arr);
    printf("p+3:%p\n", p + 3);

    for (i = 0; i < 3; ++i) {
        if (p[i] == NULL) {
            printf("is null.\n");
        }
        else {
            printf("not null.\n");
        }
    }


    chash* hash = chash_create(13,
        func_create_key_int,
        func_erase_key_int,
        func_create_val_int,
        func_erase_val_int,
        hash_int,
        func_key_cmp_int);

    int key = 10;
    int val = 11;

    chash_add(hash, &key, &val);
    chash_node_t* get_val = (int*)chash_get(hash, &key);

    if (get_val)
        printf("get_val:%d\n", *(int*)(get_val->val));

    chash_remove(hash, &key);
    get_val = (int*)chash_get(hash, &key);
    if (get_val)
        printf("get_val:%d\n", *(int*)(get_val->val));
    else {
        printf("get val  failured.\n");
    }
    printf("hello world2\n");

   // memset(apr_palloc(ht->pool, sizeof(*ht->array) * (max + 1)), 0, sizeof(*ht->array) * (max + 1))
    test_read_ini();
    return 0;
}