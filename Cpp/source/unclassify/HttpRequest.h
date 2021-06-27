#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__
#include "HttpPacker.h"
class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	void SetCallBack(void (*done_callback)(int sockfd), int msgid);
	void Request(const HttpPacker &p);

private:
	int m_sockfd;
	int m_nMsgId;
	void (*m_done_callback)(int sockfd);
};
#endif 
