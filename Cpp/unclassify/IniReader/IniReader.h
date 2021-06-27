#ifndef INI_READER_H
#define INI_READER_H
#include <map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <iostream>
#include <functional>
#include <algorithm>
using namespace std;
using std::string;
using std::map;
using std::list;
using std::vector;

string& RTrim(string& s);
string& LTrim(string& s);
string& Trim(string& s);

enum ErrorType
{
	ErrorType_NotDefine = -1,
	ErrorType_Success = 0, 
	ErrorType_OpenFailured = 1,
	ErrorType_NoSection = 2,
	ErrorType_Syntax_Error = 3,
	ErrorType_Repeat_Key = 4,
	ErrorType_Empty_Section_val = 5
};

typedef string SECTION_NAME_TYPE;
typedef string KEY_TYPE;
typedef string VALUE_TYPE;

typedef enum ItemType
{
	ITEM_TYPE_UNDEFINE = -1,
	ITEM_TYPE_SECTION = 1,
	ITEM_TYPE_PARAMETER = 2,///< key ,value pair
	ITEM_TYPE_COMMENTS = 3,
	ITEM_TYPE_EMPTY = 4

}ItemType;

typedef struct IniItem
{
	ItemType type;
	string source; ///
	string sectionname;
	string key;
	string value;
}IniItem;

class IniReader
{
public:
	IniReader(const string& strfile, const char comment='#');
	~IniReader();
	/**
	 * @brief
	 * @return string empty success, either error info.
	 */
	string ParseIniFile();
	string GetOptValue(const char* section, const char* szname, const string& strdefault);
	
	/** 
	 * @brief 
	 */
	bool AddSection(const char* section, const char* comments);

	/**
	 * @brief if no section , then add. if not exist key in section, then add;
	 */
	bool Set(const char* section, const char* key, const char* val,  
		     const string& comments_para="");
	
	bool WriteToFile(const string& filename = "");
	string DoShow();
private:
	string getErrorInfo(int errnum);

private:
	/** */
	char comment_char_;
	string filename_;
	vector<IniItem> configs_;
	map< SECTION_NAME_TYPE, map<KEY_TYPE, VALUE_TYPE>> dicts_;
};

#endif
/**
## comments
[section1]
#comment11
key1 = val1
## comments11
#comment22
key2 = val2

[section2]
key21 = val21
key22=val22


*/