#include "IniReader.h"
#include <iostream>
#include <fstream>
using std::string;
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;
using namespace std;


string& LTrim(string& s) {
	int (*func)(int) = isspace;
	string::iterator iter;
	iter = find_if(s.begin(), s.end(), std::not1(std::ptr_fun(func)));
	s.erase(s.begin(), iter);

	return s;
}

string& RTrim(string& s) {
	int (*func)(int) = isspace;
	string::reverse_iterator iter;
	iter = find_if(s.rbegin(), s.rend(), not1(ptr_fun(func)));
	s.erase(iter.base(), s.end());
	return s;
}

string& Trim(string& s) {

	return RTrim(LTrim(s));
}

IniReader::IniReader(const string& filename, const char comment):filename_(filename), comment_char_(comment)
{

}

IniReader::~IniReader()
{

}

string IniReader::ParseIniFile()
{
	int rs = ErrorType_Success;
	std::ifstream file(filename_.c_str(), ios::in);
	if (!file.is_open())	return "open file failured.";

	string strSection;
	char szline[256];
	string key;
	string value;
	int linenum = 0;
	static int const commnet_ch = comment_char_;
	cout << __FUNCTION__ << __LINE__ << endl;
	IniItem cur;
	while (!file.eof() && file.getline(szline, sizeof(szline), 0x0A))
	{
		string source = string(szline);
		linenum++;
		cur.source = source;

		string& sline = Trim(source);
		char firstCh = '\0';
		if (!sline.empty())
			firstCh = sline[0];

		switch (firstCh)
		{
		case '\0':
			cur.type = ITEM_TYPE_EMPTY;
			break;

		case '[':///< 右中括号也需要判断
			cur.type = ITEM_TYPE_SECTION;
			strSection = sline.substr(1, sline.size() - 2);
			strSection = Trim(strSection);
			if (strSection.length() == 0)
			{
				rs = ErrorType_Empty_Section_val;
				break;
			}

			cur.sectionname = strSection;
			break;

		default:
			if (firstCh == commnet_ch)
			{
				cur.type = ITEM_TYPE_COMMENTS;
				break;
			}

			if (strSection.empty())///< parameter no section
			{
				rs = ErrorType_NoSection;
				break;
			}

			cur.sectionname = strSection;
			cur.type = ITEM_TYPE_PARAMETER;

			string::size_type pos = sline.find("=");
			if (string::npos == pos)
			{
				rs = ErrorType_Syntax_Error;
				break;
			}

			string strtemp = sline.substr(0, pos);
			string key = Trim(strtemp);
			strtemp = sline.substr(pos + 1, sline.size() - pos - 1);
			string val = Trim(strtemp);

			if (key.empty() || val.empty())
			{
				rs = ErrorType_Syntax_Error;
				break;
			}

			/** check in map */
			map< SECTION_NAME_TYPE, map<KEY_TYPE, VALUE_TYPE>>::iterator it = dicts_.find(strSection);
			if (it != dicts_.end())
			{
				map<KEY_TYPE, VALUE_TYPE>::iterator it2 = it->second.find(key);
				if (it2 != it->second.end())
				{
					rs = ErrorType_Repeat_Key;
					break;
				}
			}

			cur.key = key;
			cur.value = val;
			dicts_[strSection][key] = val;
			
		}

		if (rs != ErrorType_Success)
		{
			break;
		}

		configs_.push_back(cur);
	}// end while

	if (rs != ErrorType_Success)
	{
		string errorinfo = this->getErrorInfo(rs);
		return string ("linenum:" + std::to_string(linenum) + " " + errorinfo);
	}

	return "";
}

string IniReader::getErrorInfo(int errnum)
{
	switch (errnum)
	{
	case ErrorType_NotDefine:
		return "undefine";
	case ErrorType_Success:
		return "success";
	case ErrorType_OpenFailured:
		return "open failed";
	case ErrorType_NoSection:
		return "parameter no section";
	case ErrorType_Syntax_Error:
		return "syntax error";
	case ErrorType_Repeat_Key:
		return "repeted key";
	case ErrorType_Empty_Section_val:
		return "empty section value";
	default:
		break;
	}
	return "undefine errnum";
}

string IniReader::DoShow()
{
	map<SECTION_NAME_TYPE, map<KEY_TYPE, VALUE_TYPE>>::iterator it = dicts_.begin();
	while (it != dicts_.end())
	{
		cout << it->first << endl;
		map<KEY_TYPE, VALUE_TYPE>::iterator it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			cout << it2->first << " "<< it2->second << endl;
			++it2;
		}

		++it;
	}
	return "";
}

string IniReader::GetOptValue(const char* section, const char* szname, const string& strdefault)
{
	string res;
	map<SECTION_NAME_TYPE, map<KEY_TYPE, VALUE_TYPE>>::iterator it = dicts_.find(section);
	if (it != dicts_.end())
	{
		map<KEY_TYPE, VALUE_TYPE>::iterator it2 = it->second.find(szname);
		if (it2 != it->second.end())
		{
			res = it2->second;
		}
	}
	if (res.length() == 0)
		res = strdefault;

	return res;
}

bool IniReader::WriteToFile(const string& filename)
{
	bool rs = true;
	string outfile = filename_;
	if (filename.length() != 0)
		outfile = filename;

	std::fstream file(outfile.c_str(), ios::out);
	if (!file.is_open())
		return false;

	for (int i = 0; i < configs_.size(); ++i)
	{
		file << configs_[i].source << endl;
	}

	return rs;
}

bool IniReader::Set(const char* section, const char* key, const char* val, 
					const string& comments_para)
{
	map<SECTION_NAME_TYPE, map<KEY_TYPE, VALUE_TYPE>>::iterator it_sec = dicts_.find(section);
	if (it_sec == dicts_.end()) ///< if section not exist
	{
		this->AddSection(section, "");
	}
	
	IniItem item_comment;
	item_comment.type = ITEM_TYPE_COMMENTS;
	item_comment.source = string(string("# ") + comments_para);

	it_sec = dicts_.find(section);
	map<KEY_TYPE, VALUE_TYPE>::iterator cur = it_sec->second.find(key);
	if (cur == it_sec->second.end())///< not exist
	{
		IniItem item_parameter;
		item_parameter.type = ITEM_TYPE_PARAMETER;
		item_parameter.source = string(key) + string(" = ") + string(val);
		item_parameter.key = key;
		item_parameter.value = val;
		/** insert in the last of the section */
		bool found = false;
		size_t fi = configs_.size();
		/**
			[section2]
			key21 = val21
			key22=val22
		*/
		for (size_t i = 0; i < configs_.size(); ++i)
		{
			if (configs_[i].type == ITEM_TYPE_SECTION && found)
			{
				fi = i;
				break;
			}

			if (configs_[i].type == ITEM_TYPE_SECTION && configs_[i].sectionname == section)
			{
				found = true;
			}
		}

		if (fi != configs_.size())
		{
			configs_.insert(configs_.begin() + fi, item_comment);
			configs_.insert(configs_.begin() + fi + 1, item_parameter);
		}
		else
		{
			configs_.push_back(item_comment);
			configs_.push_back(item_parameter);
		}

		it_sec->second[key] = val;
		/** add to the map */

	}
	else ///< exist
	{
		/** modify the comments */
		/** does it contain comments ? */
		vector<IniItem>::reverse_iterator it = configs_.rbegin();
		vector<IniItem>::reverse_iterator it_comments;
		while (it != configs_.rend())
		{
			if (it->type == ITEM_TYPE_PARAMETER && it->key == string(key))
			{
				it_comments = it + 1;
				break;
			}
			++it;
		}

		if (it_comments->type == ITEM_TYPE_COMMENTS)///< exist comments
		{
			it_comments->source = string("# ") + comments_para;
			vector<IniItem>::reverse_iterator next = it_comments + 1;
			while (next->type == ITEM_TYPE_COMMENTS)
			{
				next->source = string("");/// set the other commnets to empty.
				next++;
			}
		}
		else
		{
			int diff = configs_.rend() - it;
			configs_.insert(configs_.begin() + diff , item_comment);
		}

		it->value = string(val);
		it->source = string(it->key) + string(" = ") + string(val);
		dicts_[section][key] = val;
	}///< 

	return true;
}


bool IniReader::AddSection(const char* section, const char* comments)
{
	map<SECTION_NAME_TYPE, map<KEY_TYPE, VALUE_TYPE>>::iterator it = dicts_.find(section);
	if (it == dicts_.end()) ///< if section not exist
	{
		IniItem item_comments;
		item_comments.type = ITEM_TYPE_COMMENTS;
		item_comments.source = string("# ") + string(comments);
		configs_.push_back(item_comments);

		IniItem item;
		item.type = ITEM_TYPE_SECTION;
		item.source = string("[") + string(section) + string("]");
		item.sectionname = string(section);

		configs_.push_back(item);
		map<KEY_TYPE, VALUE_TYPE> empty;
		dicts_[item.sectionname] = map<KEY_TYPE, VALUE_TYPE>();
		return true;
	}

	return false;
}
