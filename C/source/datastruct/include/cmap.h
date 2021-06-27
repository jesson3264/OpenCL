#ifndef __CMAP_H__
#define __CMAP_H__

#include "dscommon.h"

#ifdef __cplusplus
extern "C" {
#endif

int key_cmp_int(void* key1, void* key2);
int value_cmp_int(void* key1, void* key2);
int hash_func_int(void *key);

#define DEFAULT_BUCKET_SIZE 13  // prime number is better.
enum KEY_TYPE { KEY_TYPE_INT, KEY_TYPE_STR, KEY_TYPE_PTR };
enum VAL_TYPE { VAL_TYPE_INT, VAL_TYPE_STR, VAL_TYPE_PTR };

typedef struct mnode
{
	union //key
	{
		int   k_integer; //  整形类型
		void* k_ptr;	 //  指针类型,字符串自己申请
	};

	union //val
	{
		int v_integer;
		void* v_ptr;	 //  指针类型,字符串自己申请
	};

	struct mnode* next;
}mnode;

typedef struct cmap_iterator
{
	int index;
	mnode* node;
}cmap_iterator;

typedef struct cmap
{
	int key_type;    // pointer, int string
	int val_type;

	int bucket_size; // arraysize
	mnode* bucket_array; //
	
	int (*key_cmp)(void* key1, void* key2);
	int (*val_cmp)(void* val1, void* val2);
	int (*hash_func)(void *key);
}cmap;

cmap* cmap_create(int key_type, int val_type, int(*key_cmp)(void* key1, void *key2),
	int(*val_cmp)(void* key1, void *key2),
	int(*hash_func)(void* key));

void cmap_init_with_size(cmap* m, int key_type, int val_type, int bucket_size, int (*cmp)(void* key1, void *key2),
	int(*val_cmp)(void* key1, void *key2),
	int(*hash_func)(void* key));

void* cmap_get(cmap* m, void *key);

mnode* cmap_find(cmap* m, void *key);

cmap_iterator* cmap_get_start_pos(cmap* m, cmap_iterator* pos);

cmap_iterator* cmap_get_next(cmap* m, cmap_iterator* pos);

// 1 新值  0， 修改之前的值
int cmap_insert(cmap* m, void* key, void* value);

void cmap_replace_value(cmap* m, mnode* mn, void* newvalue);
void cmap_create_key(cmap* m, mnode* mn, void *key);

void cmap_destroy(cmap* m);

#ifdef __cplusplus
}
#endif
#endif
