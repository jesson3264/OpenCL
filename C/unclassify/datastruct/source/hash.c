#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define T chash_node_t 

size_t default_hash_key(void* val)
{
    if (!val) {
        return 0;
    }
    size_t hash = 0;
    const char* str = (char*)val;
    const char* p;
    for (p = str; *p; ++p) {
        hash += hash * 33 + *p;
    }

    return hash;
}

chash* chash_create(size_t bucket_size,
    func_clone_key fmk,
    func_destroy_key ffk,
    func_clone_val fmv,
    func_destroy_val ffv, 
    func_hash_key fhash,
    func_key_cmp key_cmp)
{
    if (bucket_size == 0) {
        return NULL;
    }

    chash* hash = (chash*)calloc(1, sizeof(chash));
    size_t tmp = sizeof(*hash->buckets);
    T** nodes = (T*)calloc(tmp,  sizeof(T*));
    if (!hash || !nodes) {
        return NULL;
    }

    hash->buckets = (T**)nodes;
    hash->nbuckets = bucket_size;

    hash->malloc_key = fmk;
    hash->free_key = ffk;
    hash->malloc_val = fmv;
    hash->free_val = ffv;
    hash->key_cmp = key_cmp;

    if (!fhash)
        hash->fhash = default_hash_key;
    else
        hash->fhash = fhash;

    return hash;
}

T* chash_add(chash* hash, void* key, void* val)
{
    if (!key) {
        return NULL;
    }

    size_t hash_val = hash->fhash(key);
    size_t index = hash->nbuckets % hash_val;
    if (hash->buckets[index] == NULL)
    {
        printf("index : is null.\n");
    }

    T t;
    T* pre = &t;
    pre->next = hash->buckets[index];
    T* node = pre->next;
    while (node) {
        if (hash->key_cmp(node->key, key) == 0) {
            break;
        }

        pre = node;
        node = node->next;
    }

    if (node) { //exist
        if (hash->free_val) {
            hash->free_val(node->val);
        }

        node->val = val;
    } else {
        node = (T*)malloc(sizeof(T));
        node->key = key;
        node->val = val;
        if (hash->malloc_key) {
            node->key = hash->malloc_key(key);
        }

        if (hash->malloc_val) {
            node->val = hash->malloc_val(val);
        }
        
        node->next = NULL;
        node->next = pre->next;
        pre->next = node;
    }

    return node;
}

T* chash_get(chash* hash, void* key)
{
    size_t hash_val = hash->fhash(key);
    size_t index = hash->nbuckets % hash_val;
    T t;
    T* pre = &t;
    pre->next = hash->buckets[index];
    T* node = pre->next;

    while (node) {
        if (hash->key_cmp) {
            if (hash->key_cmp(key, node->key) == 0 ) {
                break;
            }
        }

        node = node->next;
    }

    return node;
}

static int chash_free_node(chash* hash, T* node)
{
    if (!hash || !node) {
        return -1;
    }

    if (hash->free_key) {
        hash->free_key(node->key);
    }

    if (hash->free_val) {
        hash->free_val(node->val);
    }

    free(node);
    return 0;
}

int chash_remove(chash* hash, void* key)
{
    size_t hash_val = hash->fhash(key);
    size_t index = hash->nbuckets % hash_val;
    // 用一个临时节点，方便编程
    T t;
    T* pre = &t;
    pre->next = hash->buckets[index];
    T* node = pre->next;
    while (node) {
        if (hash->key_cmp) {
            if (hash->key_cmp(key, node->key) == 0) {
                break;
            }
        }

        pre = node;
        node = node->next;
    }

    if (node)
    {
        pre->next = node->next;
    }

    chash_free_node(hash, node);

    return 0;
}

int chash_destroy(chash* hash)
{
    int i = 0; 
    T pre;
    for (i = 0; i < hash->nbuckets; ++i) {
        pre.next = hash->buckets[i];
        T* node = pre.next;
        while (node) {
            pre.next = node->next;
            if (hash->free_val) {
                hash->free_val(node->val);
            }

            if (hash->free_key) {
                hash->free_key(node->key);
            }

            free(node);
            node = pre.next;
        }
    }

    free(hash->buckets);
    free(hash);
    return 0;
}

void* func_clone_key_str(void* key)
{
    const unsigned char* str = (unsigned char*)key;
    size_t len = strlen(str) + 1;
    unsigned char* t = (unsigned char*)malloc(len);
    memcpy(t, str, len);
    return (void*)t;
}

void func_erase_destroy_str(void* key)
{
    free(key);
}

void* func_clone_val_str(void* key)
{
    const unsigned char* str = (unsigned char*)key;
    size_t len = strlen(str) + 1;
    unsigned char* t = (unsigned char*)malloc(len);
    memcpy(t, str, len);
    return (void*)t;
}

void func_destroy_val_str(void* key)
{
    free(key);
}