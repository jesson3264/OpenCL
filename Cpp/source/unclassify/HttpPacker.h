#ifndef __HTTP_PACKER_H__
#define __HTTP_PACKER_H__
#include <string>
#include <map>
#include "DataNode.h"
#ifdef  IN_EMM
#define LOG  EMMRunPrintf
#else
#define LOG  printf
#endif

using std::string;
using std::map;


#define SPACE_LEN  strlen(" ")
#define HEAD_LINE_END "\r\n" 
typedef struct emmstr
{
	char *buf;
	int size;
}emmstr;
class HttpPacker
{
public:
	HttpPacker();
	~HttpPacker();
	void SetVersion(const char *version);//
	void SetMethod(const char *method);// GET POST CONNECT etc. 
	void SetUrl(const char *url);
	// 以 '\0'; 结尾
	void SetBody(const char *pszbody);
	// 非 '\0' 结尾带上 数据长度 len
	void SetBody(const char *body, int len);
	emmstr* Pack();
private:
	void toUpper(std::string &str);

	int calcTotalLen();
private:
	string m_sMethod;
	string m_sUrl;
	string m_sVersion;
	// 头部信息处请求行以为的头部信息 
	map<string, string> m_mHeaders;//Content-Length:xxx
	emmstr m_eBody;
	// 包含头部和消息体body 
	emmstr m_eHole; 
}; 



#endif 

/*
 
GET /hello/index.jsp HTTP/1.1
请求头信息，因为GET请求没有正文
Host: localhost
User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,
Accept-Language: zh-cn,zh;q=0.5
Accept-Encoding: gzip, deflate
Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7
Connection: keep-alive
Cookie: JSESSIONID=369766FDF6220F7803433C0B2DE36D98
空行
因为GET没有正文，所以下面为空
*/

/*
// 请求首行
// POST /hello/index.jsp HTTP/1.1
// //请求头信息
 Host: localhost
 User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0
 Accept: text/html,application/xhtml+xml,application/xml;q=0.9,
 Accept-Language: zh-cn,zh;q=0.5
 Accept-Encoding: gzip, deflate
 Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7
 Connection: keep-alive
 Referer: http://localhost/hello/index.jsp
 Cookie: JSESSIONID=369766FDF6220F7803433C0B2DE36D98
 Content-Type: application/x-www-form-urlencoded 
 Content-Length: 14 
 // 这里是空行
// //POST有请求正文
 username=hello
*/

