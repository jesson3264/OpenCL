#ifndef __ERROR_INFO_H__
#define __ERROR_INFO_H__
#include <string>

using std::string;
typedef struct ErrorInfo
{
    ErrorInfo()
    {
        err_no_= 0;
    }
	ErrorInfo(int e, int l, string s)
	{
		err_no_ = e;
		line_num_ = l;
		info_ = s;
	}

    int err_no_;
    int line_num_;
    std::string info_;
}ErrorInfo;

enum ErrNo
{
    //  IniReader
    ERR_INI_SYNTAX_ERR = 1,
    ERR_INI_NO_SECTION = 2

};


#endif 
