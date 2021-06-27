#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__
#ifndef SKIP_LIST_H_INCLUDED
#define SKIP_LIST_H_INCLUDED

typedef struct skip_list_s* skip_list_t;

/**
 * @return	新建的的空跳表实例
 */
skip_list_t skip_list_create();

/**
 * 销毁跳表实例，不会销毁跳表中包含的值。
 */
void skip_list_destroy(skip_list_t sl);

/**
 * 查询跳表中key对应的值。
 * 返回NULL不代表跳表中一定不包含key，以skip_list_contains(sl, key)结果为准。
 * @param	key		要查询的键，允许key在跳表中不存在。
 * @return	跳表中key对应的值
 */
void* skip_list_get(skip_list_t sl, int key);

/**
 * 向跳表中添加一个键值对，这将使得skip_list_contains(sl, key)==1。
 * 如果跳表中已经存在相同的键，则替换其旧值，否则创建一个新的键值对。
 * @param	value	key对应的新的值，允许为NULL。
 * @return	跳表中key原来对应的值
 */
void* skip_list_put(skip_list_t sl, int key, void *value);

/**
 * 从跳表中删除一个键值对，这将使得skip_list_contains(sl, key)==0。
 * @param	key		要删除的键，允许key在跳表中不存在。
 * @return	跳表中key对应的值
 */
void* skip_list_remove(skip_list_t sl, int key);

/**
 * @return	跳表中存在key则1，否则0
 */
int skip_list_contains(skip_list_t sl, int key);

/**
 * @return	跳表中键的数量
 */
int skip_list_count(skip_list_t sl);

/**
 * 检索跳表中键的集合，结果按照键升序排列
 * @param	[out] keys		用于存储键集合
 * @param	[int] length	keys数组的长度
 * @return	键的数量（=MIN(length, 跳表中所有键的数量)）
 */
int skip_list_key_set(skip_list_t sl, int keys[], int length);

/**
 * 检索跳表中值的集合，结果按照键升序排列
 * @param	[out] values	用于存储值集合
 * @param	[int] length	values数组的长度
 * @return	值的数量（=MIN(length, 跳表中所有键的数量)）
 */
int skip_list_value_set(skip_list_t sl, void *values[], int length);

#endif // SKIP_LIST_H_INCLUDED

#endif 
