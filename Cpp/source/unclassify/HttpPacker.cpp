#include "HttpPacker.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

HttpPacker::HttpPacker()
{
	m_eBody.buf = NULL;
	m_eBody.size = 0;
	m_eHole.buf = NULL;
	m_eHole.size = 0;
}

HttpPacker::~HttpPacker()
{
	if (m_eBody.buf)
	{
		free(m_eBody.buf);
	}

	if (m_eHole.buf)
	{
		free(m_eHole.buf);
	}	
}

void HttpPacker::SetVersion(const char *version)
{
	if (!version) 	return ;

	m_sVersion = std::string(version);
	this->toUpper(m_sVersion);
}

void HttpPacker::SetMethod(const char *method)// GET POST CONNECT etc. 
{
	if (!method) return ;

	m_sMethod = method;
	this->toUpper(m_sMethod);
}

void HttpPacker::SetUrl(const char *url)
{
	if (!url) 	return ;
		
	m_sUrl = std::string(url);
	this->toUpper(m_sUrl);
}
	
emmstr* HttpPacker::Pack()
{
	int totalLen = calcTotalLen();
	if (m_eHole.buf)
	{
		free(m_eHole.buf);
		m_eHole.buf  = NULL;
		m_eHole.size = 0;
	}
	m_eHole.size = totalLen;
	m_eHole.buf = (char *) malloc(m_eHole.size);
	if (!m_eHole.buf)
		return NULL;

	int spos = 0;
	char *buf = m_eHole.buf;

	memcpy(buf + spos, m_sMethod.data(), m_sMethod.length());
	spos += m_sMethod.length();

	memcpy(buf + spos, " ", SPACE_LEN);
	spos += SPACE_LEN;

	memcpy(buf + spos, m_sUrl.data(), m_sUrl.length());
	spos += m_sUrl.length();

	memcpy(buf + spos, " ", SPACE_LEN);
	spos += SPACE_LEN;

	memcpy(buf + spos, "HTTP/", strlen("HTTP/"));
	spos += strlen("HTTP/");

	memcpy(buf + spos, m_sVersion.data(), m_sVersion.length());
	spos += m_sVersion.length();
	// �����н���
	memcpy(buf + spos, "\r\n", strlen("\r\n"));
	spos += strlen("\r\n");


	// headers;
	for (map<string, string>::iterator it = m_mHeaders.begin(); it != m_mHeaders.end(); ++it)
	{
		memcpy(buf + spos, it->first.data(), it->first.length());
		spos += it->first.length();

		memcpy(buf + spos, ":", strlen(":"));
		spos += strlen(":");

		memcpy(buf + spos, it->second.data(), it->second.length());
		spos += it->second.length();

		memcpy(buf + spos, "\r\n", strlen("\r\n"));
		spos += strlen("\r\n");
	}

	// Content-length:xxx	
	do {
		if (!m_eBody.buf || 0 == m_eBody.size)
		{
			break;
		}
		
		memcpy(buf + spos, "Content-Length", strlen("Content-Length"));
		spos += strlen("Content-Length");

		memcpy(buf + spos, ":", strlen(":"));
		spos += strlen(":");

		char lenStr[20];
		memset(lenStr, 0, sizeof(lenStr));
		snprintf(lenStr, sizeof(lenStr), "%d", m_eBody.size);
		
		memcpy(buf + spos, lenStr, strlen(lenStr));
		spos += strlen(lenStr);

		memcpy(buf + spos, "\r\n", strlen("\r\n"));
		spos += strlen("\r\n");

	} while (0);
	
	
	// ����һ������, ��ʾ����������� body
	memcpy(buf + spos, "\r\n", strlen("\r\n"));
	spos += strlen("\r\n");

	if (m_eBody.buf && (0 != m_eBody.size))
	{
		// ��� body	
		memcpy(buf + spos, m_eBody.buf, m_eBody.size);	
		spos += m_eBody.size;
	}

	return &m_eHole;
}

int HttpPacker::calcTotalLen()
{
	// 1. �ȼ����ܳ����ٽ��пռ������
	int totalLen = 0;
	// ������
	totalLen += m_sMethod.length();// POST
	totalLen += SPACE_LEN;// �ո� // POST 
	totalLen += m_sUrl.length();// POST /hello/index.jsp
	totalLen += SPACE_LEN;
	totalLen += strlen("HTTP/");// POST /hello/index.jsp HTTP/
	totalLen += m_sVersion.length();// POST /hello/index.jsp HTTP/1.1
	totalLen += strlen("\r\n");

	for (map<string, string>::iterator it = m_mHeaders.begin(); it != m_mHeaders.end(); ++it)
	{
		totalLen += it->first.length();
		totalLen += strlen(":");
		totalLen += it->second.length();
		totalLen += strlen("\r\n");
	}
	
	// content-length:
	do {
		if (!m_eBody.buf || 0 == m_eBody.size)
		{
			break;
		}

		totalLen += strlen("Content-Length");
		totalLen += strlen(":");

		char lenStr[20];
		memset(lenStr, 0, sizeof(lenStr));
		snprintf(lenStr, sizeof(lenStr), "%d", m_eBody.size);

		totalLen += strlen(lenStr);
		totalLen += strlen("\r\n");

	} while (0);

	totalLen += strlen("\r\n");
	if (m_eBody.buf)
	{
		totalLen += m_eBody.size;
	}

	return totalLen;
}

void HttpPacker::toUpper(std::string &str)
{
	for (size_t i = 0; i < str.length(); ++i)
		toupper(str[i]);
}

void HttpPacker::SetBody(const char *pszbody)
{
	if (!pszbody)	return ;
	
	SetBody(pszbody, strlen(pszbody));
}

void HttpPacker::SetBody(const char *body, int len)
{
	if (!body || 0 >= len)	return ;

	if (m_eBody.buf)
	{
		free(m_eBody.buf);
	}

	m_eBody.size = len;
	m_eBody.buf = (char *)malloc(m_eBody.size);
	memcpy(m_eBody.buf, body, len);
	return ;
}


