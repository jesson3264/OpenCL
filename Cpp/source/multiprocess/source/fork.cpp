#include "fork.h"
#include <unistd.h>
#include <stdio.h>

void fork_entry(int argc, char** argv)
{
	while (1)
	{
		printf("in fork_entry.\n");
		sleep(2);	
	}
}

int fork_main(int argc, char** argv, void (*func)(int argc2, char** argv2))
{
	printf("%s:%d", __FUNCTION__, __LINE__);

	pid_t fpid = fork ();
	if (fpid < 0)
	{
	}
	else if (fpid == 0)
	{
		printf("child:%d", fpid);
		func(argc, argv);
	}

	printf("parent");
	while (1)
	{
		printf("in paraent..\n");
		sleep(2);	
	}
	
	return 0;
}



#ifndef CREATE_LIBS
int main(int argc, char** argv)
{
	fork_main(argc, argv, fork_entry);
	printf("hello world.\n");
	return 0;
}
#endif
