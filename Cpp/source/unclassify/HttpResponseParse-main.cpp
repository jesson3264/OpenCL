#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h> 
#include <algorithm>
#include <iostream>
#include "HttpResponseParse.h"
#include "HttpPacker.h"
#include "DataNode.h"
#include "HttpResponseParse-main.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
int GenSession(char *dst, int dstlen)
{
	if (!dst || dstlen <= 0)
		return -1;
	#if 0
	time_t time_seconds = time(0);
	struct tm now_time;
	//localtime_r(&now_time,&time_seconds);
 	localtime_r(&time_seconds, &now_time);
	snprintf(dst, dstlen, "%d-%d-%d %d:%d:%d\n", now_time.tm_year + 1900, now_time.tm_mon + 1,
		now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
	printf("dst:%s\n");
	
	#endif
	
	struct timeval start;//, end;
	gettimeofday(&start, NULL);
	snprintf(dst, dstlen, "%ld:%ld", start.tv_sec, start.tv_usec); 
	printf("session:%s\n", dst); 
	return 1;	
}

int HttpPackerTest(int argc, char** argv) 
{

	HttpPacker  packer;
	packer.SetMethod("GET");
	packer.SetUrl("/hello/index.html");
	packer.SetVersion("1.1");
	#if 1
	emmstr tmp;
	emmstr *p = packer.Pack();
	tmp.buf = (char *)malloc(p->size + 1);
	tmp.buf[p->size] = 0;
	
	memcpy(tmp.buf, p->buf, p->size);
	printf("res:%s\n", p->buf);
	#endif 
	
	printf("test use class DataNode.\n");
	DS::DataNode dn(4096);
	
	//dn.PushData("HelloWorld", strlen("HelloWorld"));
	printf("capacity:%d datalen:%d spacelen:%d\n", 
		dn.GetCapacity(), dn.GetDataLen(), dn.GetSpaceLen());
	
	
	memcpy(dn.GetBuff(), "HelloWorld", std::min(strlen("HelloWorld"), (size_t )dn.GetSpaceLen()));
	dn.AdjustLen(std::min(strlen("HelloWorld"), (size_t )dn.GetSpaceLen()));	
	printf("capacity:%d datalen:%d spacelen:%d\n", 
		dn.GetCapacity(), dn.GetDataLen(), dn.GetSpaceLen());
	
	
	printf("test HttpResponseParse........\n");
	HttpResponseParse  parse(512*1024);
	DS::DataNode *dn2 = parse.GetDataNode();
	
	string str =
				"HTTP/1.1 200  OK\r\n"
				"Host:100\r\n"
				"Transfer-Encoding:chunked\r\n"
				"\r\n"
				"70\r\n"
				"{\"message\":{\"status\":2000,\"msg\":\"true\",\"strusername\":\"djx\",\"uidmobiledevid\":\"578d25e41a5114d5731c493b819732bb\"}}"
				"\r\n"
				"0\r\n"
				"\r\n";
	printf("%s\n", str.c_str());
	dn2->PushData(str.c_str(), str.length());
	
	parse.Parse();
	cout<<"status:"<<parse.GetPhase()<<endl; 
	cout<<"out: headers........."<<endl;
	cout<<"http code :"<<parse.GetHttpCode()<<endl;
	std::map<string, string> &headers = parse.GetHeaders();
	for (std::map<string, string>::iterator it = headers.begin(); it != headers.end(); ++it)
	{
		cout<<it->first<<":"<< it->second<<endl;
	}
	
	cout<<"out: body........."<<endl;
	std::map<int, DS::DataNode *> &bodys = parse.GetBodys();
	for (std::map<int, DS::DataNode *>::iterator it = bodys.begin(); it != bodys.end(); ++it)
	{
		cout<<it->first<<":"<< "len:"<< it->second->GetDataLen()<<it->second->GetBuff()<<endl;
	}
	
	//li1 = strtol (szNumbers,&pEnd,10);dn
	string str2 = "70";
	
	long int li1 = strtol (str2.c_str(), NULL,16);
	printf("li1: %ld\n", li1);
	system("pause");
	
	return 0;
}
