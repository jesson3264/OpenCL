#ifndef __TLV_PACKER_H__
#define __TLV_PACKER_H__
#include "cstr.h"

class TLVPacker
{
public:
	TLVPacker();
	~TLVPacker();

	cstr* GetData();
	int PushInt(int iv);
	int PushString(char *strv);
private:
	int calcLen();
private:
	int msgid;
	

};

#endif
