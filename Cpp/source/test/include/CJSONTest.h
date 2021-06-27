#ifndef __CJSONTEST_H__
#define __CJSONTEST_H__
#include <stdlib.h>
#include <stdio.h> 
#include "CJson.h"
#ifdef __cplusplus
extern "C"
{
#endif 

void DoTest();
void DoTest2();
cJSON* TestJson();
cJSON* parseJson(cJSON *root);

#ifdef __cplusplus
}
#endif 

#endif 
