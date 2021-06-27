#ifndef __INI_READER_H__
#define __INI_READER_H__
#include "hash.h"

typedef struct IniFile
{
    chash* hash;

}IniFile;

IniFile* inifile_open(const char* filepath);

/*
 * @
*/
int inifile_close(IniFile* filepath);

#endif 