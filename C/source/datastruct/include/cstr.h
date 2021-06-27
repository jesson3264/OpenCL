/*
ctr 便于字符和字节流的操作
*/
#ifndef __CSTR_H__
#define __CSTR_H__

typedef struct cstr
{
	char* data;
	int len;
}cstr;

#endif