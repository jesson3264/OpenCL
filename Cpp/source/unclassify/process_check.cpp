#include "process_check.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int process_check(const char *process_name, int len, int pid)
{
	FILE *ptr;
	char buff[512] = {0};
	char ps[128] = { 0 };
	sprintf(ps,"ps -e | grep %s | grep %d |wc -l",process_name, pid);
	strcpy(buff,"ABNORMAL");
	int val = -1;
	if((ptr=popen(ps, "r")) != NULL)
	{
		while (fgets(buff, 512, ptr) != NULL)
		{
			val = atoi(buff);
			if(val>=2)
			{
				pclose(ptr);
				return -1;
			}
		}
	}
	if(strcmp(buff,"ABNORMAL")==0)  /*ps command error*/
		return -1;        
	pclose(ptr);
	return val;
}

#if 0
int main(int argc, char **arv)
{

	int ret = process_check("telnet", strlen("telnet"), atoi(arv[1]));
	printf("ret:%d\n", ret);
	return 1;
}
#endif
