#include "ini_reader.h"

void func_erase_val_chash_s_s(void* val)
{
    chash_destroy((chash*)val)
}

IniFile* inifile_open(const char* filepath)
{
	IniFile* ini = NULL;
	ini = (IniFile*)malloc(sizeof(IniFile));

	chash* hash = chash_create(13, 
		func_create_key_str,
		func_erase_val_str,
		NULL, //不复制值
        func_erase_val_chash_s_s,
        func_hash_key_str,
        func_key_cmp_str);
    

	return ini;
}

int inifile_close(IniFile* ini)
{
	return 0;
}
