#include "testcmap.h"
#include "cmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int key_cmp_int(void* key1, void* key2)
{
	return *(int*)key1 - *(int*)key2;
}

int value_cmp_int(void* key1, void* key2)
{
	return *(int*)key1 - *(int*)key2;
}

int cmp_str(void* key1, void* key2)
{
	return strcmp((char*)key1, (char*)key2);
}

int hash_func_int(void *key)
{
	return *(int *)(key);
}

int hash_func_str(void *key)
{
	char* v = (char*)key;
	int s = 0;
	while (*v)
		s += *v++;

	return s;
}

typedef struct student
{
	char name[20];
	int age;
	int grade;

}student;

int hash_func_stu(void *key)
{
	return ((student*)key)->age;
}

int cmp_stu(void* key1, void* key2)
{
	return strcmp(((student*)key1)->name, ((student*)key2)->name);
}

void testmap()
{
#ifdef TEST_INT_INT
	cmap* m = cmap_create(KEY_TYPE_INT, VAL_TYPE_INT, key_cmp_int, value_cmp_int, hash_func_int);
	int key = 10;
	int val = 20;
	cmap_insert(m, &key, &val);
	mnode* mm = cmap_find(m, &key);
	printf("val:%d\n", mm->v_integer);
#endif
#if TEST_STR_STR
	cmap* mstr = cmap_create(KEY_TYPE_STR, VAL_TYPE_STR, cmp_str, cmp_str, hash_func_str);
	
	char *key_str = "hello";
	char *val_str = "world";
	cmap_insert(mstr, key_str, val_str);

	mnode* mnstr = cmap_find(mstr, key_str);

	cmap_insert(mstr, "shanshan", "laoye");
	mnstr = cmap_find(mstr, "shanshan");
	cmap_insert(mstr, "shanshan", "lady");
	printf("res:%s\n", mnstr->v_ptr);

	cmap_iterator iter;
	iter.node = NULL;
	cmap_get_start_pos(mstr, &iter);
	while (iter.node)
	{
		printf("key:%s val:%s\n", iter.node->k_ptr, iter.node->v_ptr);
		cmap_get_next(mstr, &iter);
	}
	cmap_destroy(mstr);
#endif
	
#if TEST_STR_PTR
	cmap* mstu = cmap_create(KEY_TYPE_STR, VAL_TYPE_PTR, cmp_str, cmp_stu, hash_func_str);
	student* stu1 = (student*)malloc(sizeof(student));
	strcpy(stu1->name, "shanshan", sizeof("shanshan"));
	cmap_insert(mstu, stu1->name, stu1);
	stu1->age = 19;
	stu1->grade = 1;

	student* stu2 = (student*)malloc(sizeof(student));
	strcpy(stu2->name, "zhangsan", sizeof("zhangsan"));
	cmap_insert(mstu, stu2->name, stu2);
	stu2->age = 20;
	stu2->grade = 2;

	cmap_iterator iter;
	iter.node = NULL;
	cmap_get_start_pos(mstu, &iter);
	student* stu = NULL;
	while (iter.node)
	{
		stu = (student*)(iter.node->v_ptr);
		printf("name:%s age:%d grade:%d\n", stu->name, stu->age, stu->grade);
		cmap_get_next(mstu, &iter);
	}
	cmap_destroy(mstu);
#endif

	cmap* mstu = cmap_create(VAL_TYPE_PTR, VAL_TYPE_PTR, cmp_stu, cmp_stu, hash_func_str);
	student* stu1 = (student*)malloc(sizeof(student));
	strcpy(stu1->name, "shanshan");
	cmap_insert(mstu, stu1, stu1);
	stu1->age = 19;
	stu1->grade = 1;

	student* stu2 = (student*)malloc(sizeof(student));
	strcpy(stu2->name, "zhangsan");
	cmap_insert(mstu, stu2, stu2);
	stu2->age = 20;
	stu2->grade = 2;

	cmap_iterator iter;
	iter.node = NULL;
	student* stu = NULL;

	cmap_get_start_pos(mstu, &iter);
	while (iter.node)
	{
		stu = (student*)(iter.node->v_ptr);
		printf("name:%s age:%d grade:%d\n", stu->name, stu->age, stu->grade);

		stu = (student*)(iter.node->k_ptr);
		printf("name:%s age:%d grade:%d\n", stu->name, stu->age, stu->grade);

		cmap_get_next(mstu, &iter);
	}

	printf(".....\n");
	stu1->age = 999;

	cmap_get_start_pos(mstu, &iter);
	while (iter.node)
	{
		stu = (student*)(iter.node->v_ptr);
		printf("name:%s age:%d grade:%d\n", stu->name, stu->age, stu->grade);
		stu = (student*)(iter.node->k_ptr);
		printf("name:%s age:%d grade:%d\n", stu->name, stu->age, stu->grade);

		cmap_get_next(mstu, &iter);
	}
	cmap_destroy(mstu);
}
