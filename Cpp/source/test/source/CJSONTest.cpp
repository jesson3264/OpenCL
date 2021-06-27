#include "CJSONTest.h"
#include "CJson.h"
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#ifdef __cplusplus
extern "C"
{
#endif
void showJson(cJSON *root);
extern void *(*cJSON_malloc)(size_t sz);
extern void (*cJSON_free)(void *ptr);
cJSON* packJsonData()
{
	cJSON *root = cJSON_CreateObject();
	
	cJSON *age = cJSON_CreateObject();
	cJSON *name = cJSON_CreateObject();
	cJSON_AddNumberToObject(age, "age", 20);
	cJSON_AddStringToObject(name, "name", "dengjunxing");
	
	cJSON_AddItemToObject(root, "age", age);
	cJSON_AddItemToObject(root, "name", name);
 	
 	printf("\nout:\n%s\n", cJSON_Print(root));
 	
 	return root;
}

void DoTest2()
{

	char value1[] = {"[\"keyinner\":\"valueinner\"]"};


	cJSON *root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "key", value1);
	char *out = cJSON_Print(root);
	printf("out:%s\n", out);
	printf("end DoTest2\n");

	// 我再取出来
	cJSON *parse = cJSON_Parse(out);

	cJSON *value_out = cJSON_GetObjectItem(parse, "key");
	printf("out value:%s\n", value_out->valuestring);
	/*
		char *data = "{\"love\": \
	[\"LOL\",\"[\"key\":\"value\"]Go shopping\"]   \
	}";
	*/
#if 0
	char *data = "{\"love\": \
	[\"LOL\",\"Go shopping\"]   \
	}";
	cJSON *root = cJSON_Parse(data);
	char *print = cJSON_Print (root);
	printf("out:%s", print);
	cJSON_Delete(root);

#endif

	
}

void DoTest()
{
	#if 0
	char *buff = (char *)malloc(10);
	memset(buff, 0, 10);
	memcpy(buff, "{}", 3);
	cJSON *rr = cJSON_Parse(buff);
	if (!rr)	printf("\nNULL");
	else printf("\n NOT NULL\n");
	cJSON_Delete(rr);
	free(buff);
	#endif
	
	cJSON *root = packJsonData();
	cJSON *newRoot = parseJson(root);
	cJSON_Delete(root);
	showJson(newRoot);
	printf("\n cjson_print newRoot:\n%s\n", cJSON_Print(newRoot));
	printf("\n cJSON_PrintUnformatted newRoot:\n%s\n", cJSON_PrintUnformatted(newRoot));
}

cJSON* parseJson(cJSON *root)
{
	if (!root)	return NULL;
	cJSON *cur = root->child;
	cJSON *newRoot = cJSON_CreateObject();
	int i = 0;
	while (cur)
	{
		printf("cur:%s\n", cur->string);
		cJSON *t = cJSON_Duplicate(cur, 1);
		
		cJSON_AddItemToObject(newRoot, cur->string, t);
		char tmp[100] = {0};
		snprintf(tmp, 100, "abc%d", i++);
		
		cJSON_free(t->string);
		t->string=cJSON_strdup(tmp);
		
		t->string[0] = 'm';
		cur = cur->next;
	}
	
	
	return newRoot;
}

void showJson(cJSON *root)
{
	printf ("start parse.\n");
	if (!root)	return ;
	cJSON *cur = root->child;
	while (cur)
	{
		printf("cur:%s\n", cur->string);
		cur = cur->next;
	}
}

#ifdef __cplusplus
}
#endif
