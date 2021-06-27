#include "EmmManager-Test.h"
#include "HttpRequest.h"
#include "cfg_reader.h"
#include <string.h>
#define INI_FILE_PATH "./msg.ini"
void done_callback(int msgid)
{
	printf("msgid:%d done\n", msgid);
}

void ReadIniFile(const char *ini_file_path)
{

}

int emm_manager_test_main(int argc, char **argv)
{

	char url[100];
	memset(url, 0, sizeof(url));
	snprintf(url, sizeof(url), "/emm-core/securityController/getGWACLConfig.json");

	HttpPacker packer;
	packer.SetMethod("POST");
	packer.SetUrl(url);
	
//	emmstr *str = packer.Pack();


	HttpRequest req;
	req.SetCallBack(done_callback, 100);
	req.Request(packer);

	ACE::CCfgReader  cfgreader(INI_FILE_PATH);
	cfgreader.show();	


	return 0;
}
