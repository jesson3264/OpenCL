#include "cmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

cmap* cmap_create(int key_type, int val_type,
				  int(*key_cmp)(void* key1, void *key2),
				  int(*val_cmp)(void* key1, void *key2),
				  int(*hash_func)(void* key))
{
	cmap* m = (cmap*)malloc(sizeof(cmap));
	cmap_init_with_size(m, key_type, val_type, DEFAULT_BUCKET_SIZE, key_cmp, val_cmp, hash_func);
	return m;
}

void cmap_init_with_size(cmap* m, int key_type, int val_type, int bucket_size, int(*key_cmp)(void* key1, void *key2),
	int(*val_cmp)(void* key1, void *key2),
	int(*hash_func)(void* key))
{
	int i = 0;
	m->key_cmp = key_cmp;
	m->val_cmp = val_cmp;
	m->hash_func = hash_func;

	m->key_type = key_type;
	m->val_type = val_type;

	m->bucket_size = bucket_size;
	m->bucket_array = (mnode*)malloc(m->bucket_size * sizeof(mnode));
	for (i = 0; i < m->bucket_size; ++i)
		m->bucket_array[i].next = NULL;
}

void cmap_set(cmap* m, void *key)
{
	// struct mnode * newnode = (struct mnode*)malloc(sizeof(struct mnode));
	
	if (m->key_type == KEY_TYPE_INT)
	{
		;
	}
}

void* cmap_get(cmap* m, void *key)
{
	int found = 0;
	int idx = m->hash_func(key) % m->bucket_size;
	struct mnode* pre = &m->bucket_array[idx];
	struct mnode* cur = m->bucket_array[idx].next;
	while (cur)
	{
		cur = cur->next;
	}
	return NULL;

}
// 1 新节点  0， 修改之前的值
int cmap_insert(cmap* m, void* key, void* value)
{
	int ret = 0;
	int idx = m->hash_func(key) % m->bucket_size;
	struct mnode* pre = &m->bucket_array[idx];
	struct mnode* cur = pre->next;
	while (cur)
	{
		if (KEY_TYPE_INT == m->key_type)
		{
			if (0 == m->key_cmp(&cur->k_integer, key))
			{
				cmap_replace_value(m, cur, value);
				break;
			}
		}
		else
		{
			if (0 == m->key_cmp(cur->k_ptr, key))
			{
				cmap_replace_value(m, cur, value);
				break;
			}
		}
		pre = cur;
		cur->next;
	}

	if (cur == NULL)
	{
		ret = 1;
		// 新建一个节点插入新的值
		struct mnode *n = (mnode*)malloc(sizeof(mnode));
		n->k_ptr = NULL;
		n->v_ptr = NULL;
		cmap_create_key(m, n, key);
		cmap_replace_value(m, n, value);
		
		pre->next = n;
		n->next = NULL;
	}

	return ret;
}

mnode* cmap_find(cmap* m, void *key)
{
	mnode *ptr = NULL;
	int idx = 0;
	int hash_val = m->hash_func(key);
	idx = hash_val % m->bucket_size;
	mnode* i = m->bucket_array[idx].next;
	while (i)
	{
		if (KEY_TYPE_INT == m->key_type)
		{
			if (m->key_cmp(&i->k_integer, key)==0)
			{
				printf("equal\n");
				ptr = i;
				break;
			}
		}
		else
		{
			if (m->key_cmp(i->k_ptr, key) == 0)
			{
				printf("equal..\n");
				ptr = i;
				break;
			}
		}
			
		i = i->next;
	}

	return ptr;
}

void cmap_replace_value(cmap* m, mnode* mn, void* newvalue)
{
	// 字符串要释放之前的空间
	if (VAL_TYPE_INT == m->val_type)
	{
		mn->v_integer = *(int*)newvalue;
	}
	else if(VAL_TYPE_STR == m->val_type)
	{
		if (mn->v_ptr)
			free(mn->v_ptr);
		int sz = strlen((char*)newvalue) + 1;
		char *v = (char*)malloc(strlen((char*)newvalue) + 1);
		memset(v, 0, sz);
		memcpy(v, (char*)newvalue, sz);
		mn->v_ptr = (void*)v;
	}
	else if (VAL_TYPE_PTR == m->val_type)
	{
		mn->v_ptr = newvalue;
	}
}

void cmap_create_key(cmap* m, mnode* mn, void *key)
{
	if (KEY_TYPE_INT == m->key_type)
	{
		mn->k_integer = *(int*)key;
	}
	else if (KEY_TYPE_STR == m->key_type)
	{
		int sz = strlen((char*)key) + 1;
		char* newkey = (char*)malloc(sz);
		memcpy(newkey, (char*)key, sz);
		mn->k_ptr = (void*)newkey;
	}
	else if (KEY_TYPE_PTR == m->key_type)
	{
		mn->k_ptr = key;
	}
}

void cmap_destroy(cmap* m)
{
	/* 
	1. 先销毁节点定义 中的 k_ptr 和 v_ptr 
	2. 销毁节点
	3. 最后销毁 cmap 
	*/
	int i = 0;
	mnode* mn = NULL;
	mnode* next = NULL;
	mnode* cur = NULL;
	for (i = 0; i < m->bucket_size; ++i)
	{
		mn = &m->bucket_array[i];
		cur = mn->next;
		while (cur)
		{
			next = cur->next;

			if (m->key_type == KEY_TYPE_STR)
			{
				free(cur->k_ptr);
			}
			if (m->val_type == VAL_TYPE_STR)
			{
				free(cur->v_ptr);
			}

			cur = next;
		}
	}

	free(m->bucket_array);
	free(m);
}

cmap_iterator* cmap_get_start_pos(cmap* m, cmap_iterator* pos)
{
	// 找到第一个有值的地方
	// 不存在返回空
	int i = 0;
	mnode* mn = NULL;
	pos->node = NULL;
	for (i = 0; i < m->bucket_size; ++i)
	{
		mn = &m->bucket_array[i];
		if (mn->next)
		{
			(pos)->index = i;
			(pos)->node = mn->next;
			break;
		}
	}

	return pos;
}

cmap_iterator* cmap_get_next(cmap* m, cmap_iterator* pos)
{
	if (NULL == pos || pos->node == NULL)
		return NULL;
	int i = 0;
	int idx = pos->index;
	mnode* cur = pos->node;
	pos->node = NULL;
	if (cur->next)
	{
		pos->node = cur->next;
	}
	else
	{	// 从下一个桶里找。
		for (i = idx + 1; i < m->bucket_size; ++i)
		{
			cur = &m->bucket_array[i];
			if (cur->next)
			{
				pos->index = i;
				pos->node = cur->next;
				break;
			}
		}
	}
	return pos;
}