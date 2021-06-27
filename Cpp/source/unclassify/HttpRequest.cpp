#include "HttpRequest.h"
#include <iostream>
using namespace std;
HttpRequest::HttpRequest()
{


}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::Request(const HttpPacker &p)
{
	m_done_callback(m_nMsgId);	
}

void HttpRequest::SetCallBack(void (*done_callback)(int sockfd), int msgid)
{
	m_done_callback = done_callback;
	m_nMsgId = msgid;
}
