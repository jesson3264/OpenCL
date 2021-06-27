#include "HttpResponseParse.h"
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "./xFile/CJson.h"
using std::vector;
HttpResponseParse::HttpResponseParse()
{
	m_pdDn = new DS::DataNode();
	m_hStatus = HttpParse_Start;
	m_nHandledEnd = 0;
	m_nBodySerivalNo = 0;
	m_nHttpCode = -1;
}

HttpResponseParse::HttpResponseParse(int dn_len)
{
	m_pdDn = new DS::DataNode(dn_len);
	m_hStatus = HttpParse_Start;
	m_nHandledEnd = 0; //
	m_nBodySerivalNo = 0;
	m_nHttpCode = -1;
}

HttpResponseParse::~HttpResponseParse()
{
	if (m_pdDn)
	{
		delete m_pdDn;
	}
	
	std::vector<DS::DataNode*>  v;
	for (std::map<int, DS::DataNode *>::iterator it = m_mBodys.begin(); it != m_mBodys.end(); ++it)
		v.push_back(it->second);	
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i])
			delete v[i];
	}
} 

const string& HttpResponseParse::GetResponseLine()
{
	return m_sResponseLine;
}

HttpParsePhase HttpResponseParse::GetPhase()
{
	return m_hStatus;	
}

int HttpResponseParse::Parse()
{
	int doContinue = 0; // 表示可以获取下一阶段的数据 
	int ret = 0;
	if (m_hStatus == HttpParse_End)
	{
		return 1;
	}
	
	do //(doContinue) 
	{
		doContinue = 0;
		switch (m_hStatus)
		{
		case HttpParse_Start:
			ret = parseResponse();
			if (ret)
			{	
				doContinue = 1;
				m_hStatus = HttpParse_Response;
			}	
			
			break;
		case HttpParse_Response:
			ret = this->parseHeaders();
			if (ret)
			{
				doContinue = 1;
				m_hStatus = HttpParse_Headers;
			}
			
			break;
		case HttpParse_Headers:
			ret = this->parseBody();
			if (ret)
			{
				doContinue = 1;
				m_hStatus = HttpParse_Body;
			}
			
			break;
		case HttpParse_Body:
			m_hStatus = HttpParse_End;
			break;
		case HttpParse_End:
			
			break;
		default :
				break;
		} // switch (m_hStatus)	
		
		if (!doContinue)
		{
			break;
		}
	} while (1);
	
	return 1;
}
/*返回 1， 表示获取到了， */ 
int HttpResponseParse::parseResponse()
{
	// 直接使用  strstr(str, "\r\n")  可能导致str越界查找 
	char *str = m_pdDn->GetBuff();
	int dlen = m_pdDn->GetDataLen();
	for (int i = 0; i < dlen - 1; ++i)
	{
		if ((emm_str2_cmp((str+i), '\r', '\n')))
		{
			//m_psHandledEnd = str + i + 2;
			m_nHandledEnd = i + 2;
			
			char *sp = NULL, *ep = NULL;
			for (int j = 0; j < i; ++j)
			{
				if (*(str + j) == ' ')	
				{
					if (!sp)
					{
						sp = str + j;
						continue;	
					}
					
					ep = str + j;
					break;	
				}
			}
			
			int s = 0;
			for ( char *p = sp + 1; p < ep; ++p)
			{
				if (isdigit(*p))
				{
					s *= 10;
					s += *p - '0';
				}
			}
			
			m_nHttpCode = s;
			return 1;
		}
	}
	
	return 0;
}

int HttpResponseParse::parseHeaders()
{
	char *startStr = m_pdDn->GetBuff();
	int dlen = m_pdDn->GetDataLen();
	// 已分析位置到数据尾部的长度 
	int surpluslen = dlen - m_nHandledEnd;
	char *searcher = m_nHandledEnd + startStr;//
	char *findEnd = NULL;
	int found = 0;
	for (int i = 0; i < surpluslen - 3; ++i)
	{
		if ((emm_str4_cmp((searcher + i), '\r','\n','\r','\n')))
		{
			findEnd = searcher + i + 4;
			found = 1;
			break; 
		}
	}
	
	if (found)
	{
		// 解析所有的请求行 
		this->parseHeaderItems(m_nHandledEnd + startStr, findEnd);
		m_nHandledEnd = (findEnd - startStr);
		return 1;
	}
	
	return 0;
}

int HttpResponseParse::parseHeaderItems(const char *start, const char *end)
{
	// 
	if (!start || !end || start >= end)
	{
		return 0;
	}
	
	const char *hstart = start;
	while (1)
	{
		int dlen = end - hstart;	
		int found = 0;
		std::string key;
		std::string val;
		for (int i = 0; i < dlen; ++i)
		{
			if (hstart[i] == ':')
			{
				key = std::string(hstart, i);
				found = 1;
				hstart += i + 1;
				break;
			}
		}
		
		if (!found)
		{
			break;
		}
		found = 0;
		dlen = end - hstart;
		for (int i = 0; i < dlen - 1; ++i)
		{
			if (hstart[i] == '\r' && hstart[i+1] == '\n')
			{
				val = std::string(hstart, i);
				found = 1;
				hstart += i + 2;
				break;
			}
		}
		if (!found) break;
		m_mHeaders.insert( std::pair<std::string,std::string>(key, val));
	}
	
	return 1;
}

int HttpResponseParse::parseBody()
{
	
	if (m_mHeaders.find(string("Content-Length")) != m_mHeaders.end())
	{
		return this->parseBodyByContentLength();
	}
	else if (m_mHeaders.find("Transfer-Encoding") != m_mHeaders.end())
	{
		return this->parseBodyByTransferEncoding();
	}
	
	std::map<string, string>::iterator it = m_mHeaders.find("Content-Type");
	if (it == m_mHeaders.end())
		return 0;
	if (it->second.find("application/json") != std::string::npos)
	{
		return 	this->parseJson();
	} 
	else if (it->second.find("text/html") != std::string::npos)
	{
		return this->parseBodyHtml();
	}
	

	return 0;
}

int HttpResponseParse::parseBodyHtml()
{
	return 0;
}

int HttpResponseParse::parseJson()
{
	char *startStr = m_pdDn->GetBuff();
	int dlen = m_pdDn->GetDataLen();
	// 已分析位置到数据尾部的长度 
	printf("dlen:%d m_nHandledEnd:%d \n", dlen, m_nHandledEnd);
	int surpluslen = dlen - m_nHandledEnd;//startStr - m_psHandledEnd;
	char* pJsonString = (char*)malloc(surpluslen);
	memset(pJsonString, 0, surpluslen);
	memcpy(pJsonString, startStr + m_nHandledEnd, surpluslen);
	cJSON* pRoot = cJSON_Parse(pJsonString);
	if (!pRoot)
	{
		free(pJsonString);
		return 0;
	}
	DS::DataNode *dn = new DS::DataNode(surpluslen);
	memcpy(dn->GetBuff(), startStr + m_nHandledEnd, surpluslen);
	dn->AdjustLen(surpluslen);
	m_mBodys.insert(std::pair<int, DS::DataNode *>(m_nBodySerivalNo, dn));
	m_nBodySerivalNo++;

	free(pJsonString);
	cJSON_Delete(pRoot);
	return 1;
}

int HttpResponseParse::parseBodyByContentLength()
{
	return 1;
}

int HttpResponseParse::parseBodyByTransferEncoding()
{
	// 最后一行是  0\r\n\r\n	
	char *startStr = m_pdDn->GetBuff();
	int dlen = m_pdDn->GetDataLen();
	// 已分析位置到数据尾部的长度 
	int surpluslen = dlen - m_nHandledEnd;//startStr - m_psHandledEnd;
	char *dataEnd = startStr + dlen;// 数据结尾 
	char *handleStart = startStr + m_nHandledEnd;
	std::string key;
	std::string val;
	// int serivalNo = 0;
	int found = 0;
	int finished = 0;
	while (1)
	{
		dlen = dataEnd - handleStart;	
		found = 0;
		for (int i = 0; i < dlen - 1; ++i)
		{
			if (handleStart[i] == '\r' && handleStart[i+1] == '\n')
			{
				key = std::string(handleStart, i);
				printf("key:%s\n", key.c_str());
				found = 1;
				handleStart += i + 2;// 越过 \r\n 
				break;
			}
		}
		
		if (!found)	break;
		
		if (found)
		{
			int len = strtol (key.c_str(), NULL,16);
			if (len < 0)
			{
				found = 0;
				break;
			}
			
			surpluslen = dataEnd - handleStart;
			if (len == 0) 
			{
				if (surpluslen == 2)//\r\n
				{
					finished = 1;
					break;	
				}
			}
			else if (len > 0 )
			{
				if (surpluslen >= len + 2) // 要检查剩余数据 2->\r\n
				{
					found = 1;
					DS::DataNode *dn = new DS::DataNode(len);
					memcpy(dn->GetBuff(), handleStart, len);
					dn->AdjustLen(len);
					m_mBodys.insert(std::pair<int, DS::DataNode *>(m_nBodySerivalNo, dn));
					m_nBodySerivalNo++;
					
					handleStart += len;
					m_nHandledEnd = (handleStart - startStr);
				}
				else 
				{
					found = 0;
					break;
				}
			}//
		}// found == 1  
	} // while (1)

	if (finished)
	{
		return 1;
	}
	
	if (found == 0)
	{
		return 0;
	}
	
	return 1;
}

int HttpResponseParse::PushData(const char *src, int len)
{
	// m_pdDn->PushData 有可能做 marshal 操作，导致 m_psHandledEnd 指向的数据不对，
	// 所以先记录处理过的长度 
	int realPush = 0;
	realPush = m_pdDn->PushData(src,len);
	return realPush;
}
