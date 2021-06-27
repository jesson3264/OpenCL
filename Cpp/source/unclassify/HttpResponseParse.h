#ifndef __HTTP_RESPONSE_PARSE_H__
#define __HTTP_RESPONSE_PARSE_H__
#include <string>
#include <map>
#include "DataNode.h" 
using std::string;

/*
对回应的解析 
*/

#define emm_str2_cmp(m, c0, c1) 				\
	m[0] == c0 && m[1] == c1
#define emm_str3_cmp(m, c0, c1, c2)             \
    m[0] == c0 && m[1] == c1 && m[2] == c2
#define emm_str4_cmp(m, c0, c1, c2, c3)			\
	m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3
enum  HttpParsePhase
{
	HttpParse_Start = 0, 	// 还未获取到回应行 
	HttpParse_Response, // 获取到了 HTTP/1.1 200 OK
	HttpParse_Headers, 	// 获取到了所有 headers 
	HttpParse_Body,		// 获取到了 body 
	HttpParse_End,     	// 获取到了所有数据 
}; 


class HttpResponseParse
{
public:
	HttpResponseParse();
	HttpResponseParse(int dn_len);
	~HttpResponseParse();
	
	const string& GetResponseLine();
	HttpParsePhase GetPhase();
	
	// 返回实际压入数据长度 
	int PushData(const char *src, int len);
	
	DS::DataNode *GetDataNode()
	{
		return m_pdDn; 
	} 
	std::map<string, string> & GetHeaders()
	{
		return m_mHeaders;
	}
	std::map<int, DS::DataNode *> & GetBodys()
	{
		return m_mBodys;
	}
	int GetHttpCode()
	{
		return m_nHttpCode;
	}
	// 每次调用 Parse 都不从头开始 Parse, 只从解析过的位置开始解析
	// 比如已经解析到了 GET 为请求方法，下次就解析 
	int Parse();
	
private:
	int parseResponse();
	int parseHeaders();
	int parseHeaderItems(const char *start, const char *end);
	int parseBody();
	int parseBodyByContentLength();
	int parseBodyByTransferEncoding();
	int parseJson();
	int parseBodyHtml();
private:
	DS::DataNode *m_pdDn;		// d 表示类型简写
	std::string m_sResponseLine;// HTTP/1.1 200 OK
	std::map<string, string>  m_mHeaders;
	std::map<int, DS::DataNode *> m_mBodys;
	
	enum HttpParsePhase m_hStatus;
	int m_nHandledEnd;// 已处理位置的标记，不能用 char * 记录，因为  m_pdDn 里面的数据可能做 marshal 操作 
	int m_nBodySerivalNo;
	int m_nHttpCode;
};

#endif 

