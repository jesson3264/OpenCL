/**
 * 
**/
#ifndef __DATASTRUCT_CHASH__
#define __DATASTRUCT_CHASH__

#include <stdint.h>

#define DEFAULT_SIZE 13

typedef void* (*func_clone_key)(void* key); // 用于克隆键对象

typedef void (*func_destroy_key)(void* key);   // 用于释放键对象

typedef void* (*func_clone_val)(void* val); // 用于克隆值对象

typedef void* (*func_destroy_val)(void* val);   // 用于释放值对象

typedef size_t (*func_hash_key)(void* val);   // 默认处理字符串

// 返回 0 相等
typedef int (*func_key_cmp)(void* key1, void* key2);

typedef struct chash_node_s
{
    void* key;
    void* val;
    struct chash_node_s* next;
}chash_node_t;

typedef struct chash
{
    chash_node_t** buckets; 
    size_t nbuckets;

    func_clone_key malloc_key;
    func_destroy_key free_key;

    func_clone_val malloc_val;
    func_destroy_val free_val;

    func_hash_key  fhash;
    func_key_cmp key_cmp;
}chash;

chash* chash_create(size_t bucket_size,   // 桶大小
                    func_clone_key fmk,  // 
                    func_destroy_key ffk,   // 
                    func_clone_val fmv,  //
                    func_destroy_val ffv,   //
                    func_hash_key fhash,  //
                    func_key_cmp key_cmp);//

chash_node_t* chash_add(chash* hash, void* key, void* val);

/**
 * @brief 通过 key 获取 对应的 node
 * @return NULL 失败  其他成功
**/
chash_node_t* chash_get(chash* hash, void* key);

/**
 * @brief 从 hash 中移除 key
 * @return 0 成功 其他失败
**/
int chash_remove(chash* hash, void* key);

int chash_destroy(chash* hash);


void* func_clone_key_str(void* key);

void func_destroy_key_str(void* key);

void* func_clone_val_str(void* key);

void func_destroy_val_str(void* key);

#endif