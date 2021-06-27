/***************************************************
Module: IniReaderPlus
Creator: jxdeng3264@163.com
Time: 2019-10-15
Function: read the Ini config file.
***************************************************/
#include <errno.h>
#include <iostream>
#include <fstream>
#include "IniReaderPlus.h"
#include "util_string.h"
#include "ErrorInfo.h"
using std::string;
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;
using namespace std;
#define Log  std::cout;
namespace ACE {

IniReaderPlus::IniReaderPlus(const string &strfile):_bscan(false), _strfile(strfile)
{
}

void IniReaderPlus::destroy()
{
	map<string, map<string, LineInfo*> *>::iterator iter = _cfgDict.begin();
	for (; iter != _cfgDict.end(); ++iter)
	{
		delete iter->second;
		iter->second = NULL;
	}

	std::list<LineInfo*>::iterator iter2 = _allLine.begin();
	//auto iter2 = _allLine.begin();
	for (; iter2 != _allLine.end(); ++iter2)
	{
		delete *iter2;
		*iter2 = NULL;
	}
}

int IniReaderPlus::Sync()
{
	std::ofstream file(_strfile.c_str(), ios::in);
	if (!file.is_open())	
		return -1;

	auto it = _allLine.begin();
	for (; it != _allLine.end(); ++it)
	{
		file.write((*it)->_src.c_str(), (*it)->_src.length());
		file.write("\r\n", 2);
	}
	std::cout<<errno<<endl;
	return 1;
}

IniReaderPlus::~IniReaderPlus() 
{
	this->destroy();
}

string IniReaderPlus::GetValue(const char * section, const char *szname) 
{
	if (!_bscan) {
		ScanCfgFile();
	}
	
	map<string, map<string, LineInfo*>*>::iterator iter = _cfgDict.find(std::string(section));
	if (iter != _cfgDict.end()) 
	{
		map<string, LineInfo*> &tmp = *(iter->second);
		map<string, LineInfo*>::const_iterator iter2 = tmp.find(szname);
		if (iter2 != tmp.end())
		{
			return iter2->second->_val;
		}
	}
	
	return "";
}

string IniReaderPlus::GetOptValue(const char *section, const char *szname, const string &strdefault) 
{
	string ret = GetValue(section, szname);
	return (ret=="")? strdefault:ret;
}

int IniReaderPlus::Reload()
{
	this->destroy();
	this->ScanCfgFile();
	return 1;
}

int IniReaderPlus::ScanCfgFile() 
{
	std::ifstream file(_strfile.c_str(), ios::in);
	if (!file.is_open())	return -1;
	//# 注释行
	// [] 块
	string strSection;
	char szline[256];
	string key;
	string value;
	string kv;
	int lineNum = 0;
	ErrorInfo  ei;
	cout<<__FUNCTION__<<__LINE__<<endl;
	while (!file.eof() && file.getline(szline, sizeof(szline), 0x0A)) 
	{
		string tt = string(szline);
		lineNum++;
		LineInfo *pli = new LineInfo;
		LineInfo &li = *pli;
		li._lineNum = lineNum;
		li._src = tt;

		string &sline = Trim(tt);
		char firstCh = '\0';
		if (!sline.empty())
			firstCh = sline[0];

		switch (firstCh)
		{
		case '\0':
			li._type = typ_empty;
			break;
		case '[':
			li._type = typ_section;
			strSection = sline.substr(1, sline.size()-2);
			li._sectionName = strSection;
			break;
		case '#':
			li._type = typ_comment;
			break;
		default:
			if (strSection.empty())
			{
				ei.err_no_ = ERR_INI_NO_SECTION;
				ei.line_num_ = lineNum;
				break;
			}
			li._sectionName = strSection;
			li._type = typ_field;

			string::size_type pos = sline.find("=");
			if (string::npos == pos)
			{
				ei.err_no_ = ERR_INI_SYNTAX_ERR;
				ei.line_num_ = lineNum;
				break;
			}
			
			string strtemp = sline.substr(0, pos);
			string key = Trim(strtemp);
			strtemp = sline.substr(pos + 1, sline.size() - pos - 1);
			string val = Trim(strtemp);

			if (key.empty() || val.empty())
			{
				ei.err_no_ = ERR_INI_SYNTAX_ERR;
				ei.line_num_ = lineNum;
				break;
			}

			li._key = key;
			li._val = val;
		}

		if (ei.err_no_)
		{
			break;
		}

		_allLine.push_back(pli);

	}// end while

	if (ei.err_no_)
	{
		cout<<ei.err_no_<<ei.info_<<endl;
		return -1;
	}
	// do transfer
	this->transfer();
	_bscan = true;
	return 0;
}

int IniReaderPlus::transfer()
{
	cout<<"xx"<<endl;
	for (auto it = _allLine.begin(); it != _allLine.end(); ++it)
	{
		if (typ_field != (*it)->_type)
		{
			continue;
		}
		cout<<"xx"<<endl;
		//
		map<string, map<string, LineInfo*> *>::iterator iter = _cfgDict.find((*it)->_sectionName);
		if (iter != _cfgDict.end())
		{
			(iter->second)->insert(std::make_pair((*it)->_key, *it));
			// (iter->second)->insert(std::make_pair("ss", LineInfo()));
		}
		else 
		{
			map<string, LineInfo*> *tmp = new map<string, LineInfo*>;
			tmp->insert(make_pair((*it)->_key, *it));
			_cfgDict.insert(make_pair((*it)->_sectionName, tmp));
		}
	}

	return 1;
}

void IniReaderPlus::show() 
{
	if (!_bscan) 
	{
		int ret = ScanCfgFile();
		cout<<"ret:"<<ret<<endl;
	}

	cout<<"s"<<endl;
	auto it = _cfgDict.begin();
	while (it != _cfgDict.end()) 
	{
		cout<<it->first<<endl;
		auto it_in = it->second->begin();
		while (it_in != ((it->second))->end()) 
		{
			cout<<(it_in)->first<<"="<<(it_in)->second->_val<<endl;
			++it_in;
		}
		++it;
	}
}

int IniReaderPlus::Remove(std::string &section, std::string &key)
{	
	//
	auto it = _cfgDict.find(section);
	if (it == _cfgDict.end())
		return 0;
	auto itInner = it->second->find(key);
	if (itInner == it->second->end())
		return 0;
	int lineNum = -1;
	lineNum = itInner->second->_lineNum;

	list<LineInfo*>::iterator itList = _allLine.begin();
	for (; itList != _allLine.end(); ++itList)
	{
		if (lineNum == (*itList)->_lineNum)
		{
			_allLine.erase(itList);
			return 1;
		}	
	}

	return 0;
}

int IniReaderPlus::Modify(std::string &section, std::string &key, std::string &newval)
{
	auto it = this->find(section, key);
	if (it == _allLine.end())
		return 0;
	(*it)->_val = newval;

	return 1;
}

list<LineInfo*>::iterator IniReaderPlus::find(const std::string &section, const string &key)
{

	list<LineInfo*>::iterator itList = _allLine.begin();
	for (; itList != _allLine.end(); ++itList)
	{
		if (key == (*itList)->_key)
		{
			return itList;
		}
	}

	return _allLine.end();
}

list<LineInfo*>::iterator IniReaderPlus::find(const string &section)
{
	auto it = _allLine.begin();
	for (; it != _allLine.end(); ++it)
	{
		if (typ_section == (*it)->_type)
		{
			return it;
		}
	}

	return _allLine.end();
}

// 默认在块的最后添加， mode = 1 在块的最开头添加
int IniReaderPlus::AddBaseSection(const string &section, const string &newkey, const string &newval, int mode)
{
	// 1. 判断块是否存在
	auto it = this->find(section);
	if (it == _allLine.end())
	{
		return 0;
	}

	// 确定插入位置
	auto operit = it;
	auto pre = operit;
	if (0 == mode)
	{
		while (operit != _allLine.end())
		{
			if (typ_section == (*(++operit))->_type)
			{
				break;
			}
			pre = operit;
		}
	}

	//
	/*
		int _lineNum;
		std::string _src;
		Type _type;
		std::string _sectionName;
		std::string _key;
		std::string _val;
	*/
	++pre;
	// 3. 修正后续的 lineNum
	auto m = pre;
	for (; m != _allLine.end(); ++m)
	{
		(*pre)->_lineNum++; // 
	}

	LineInfo *li = new LineInfo();
	li->_lineNum = (*pre)->_lineNum;
	li->_src = newkey + " = " + newval;
	li->_type = typ_field;
	li->_key = newkey;
	li->_val = newval; 
	// insert 是在前面插入
	_allLine.insert(pre, li);

	// 加入到 map 中
	auto mit = _cfgDict.find(section);
	if (mit != _cfgDict.end())
	{
		mit->second->insert(std::make_pair(newval, li));
	}

	return 1;
}
    // 默认加在原 key 的后面， mode = 1 在原 key 前面添加
int IniReaderPlus::AddBaseField(const string &section, const string &key, 
				const string &newkey, const string &newval, int mode)
				
{
	auto it = this->find(section, key);
	if (it == _allLine.end())
		return 0;

	auto insertPos = it;
	int lineNum = (*it)->_lineNum;
	if (0 == mode)
	{
		++insertPos;
		++lineNum;
	}
	
	LineInfo *li = new LineInfo();
	li->_lineNum = lineNum;
	li->_src = newkey + " = " + newval;
	li->_type = typ_field;
	li->_key = newkey;
	li->_val = newval;
	_allLine.insert(insertPos, li);
	auto mit = _cfgDict.find(section);
	mit->second->insert(std::make_pair(newval, li));
	return 1;
}


}// namespace ACE
