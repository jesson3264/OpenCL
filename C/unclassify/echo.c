#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>//fileno
#include <arpa/inet.h>//htons
#include <signal.h>
#include <sys/time.h>//select
#include <sys/select.h>//select

/*
 *  * 缺点：stdio是有缓冲的,select不知道stdio使用了缓冲区，他只是从read系统调用的角度指出是否有数据可读
 *   * 可能发送的速率远大于接收，当发送结束后仍就有数据要接收，所以当发送结束时shutdown(fd,SHUT_WR),FD_CLR(fileno(stdio),&rset)
 *    */

int max(int a, int b)
{
	return a>b?a:b;
}

#define MAX_SIZE 100 
void send_echo_fun(int fd) 
{
	char buf_snd[MAX_SIZE];
	char buf_rcv[MAX_SIZE]; 
	int len; 
	fd_set rset;
	FD_ZERO(&rset);
	while(1)
	{
		FD_SET(fileno(stdin),&rset);
		FD_SET(fd,&rset);
		int maxfd = max(fileno(stdin),fd) + 1;
		select(maxfd,&rset,NULL,NULL,NULL);
		if(FD_ISSET(fd,&rset))
		{
			int len = read(fd,buf_rcv,MAX_SIZE);  
			if(len == 0)
			{
				printf("rcv fin from peer\n");
				printf("tag\n");
				return;
			}
			buf_rcv[len]='\0';
			printf("%s\n",buf_rcv);
		}
		if(FD_ISSET(fileno(stdin),&rset))
		{
			char *s = fgets(buf_snd,MAX_SIZE,stdin);
			if(s == NULL)
				break;
			int n = strlen(buf_snd)-1;
			if(n>0)
			{
				write(fd,buf_snd,n);
			}
		}
	}
}


int main()
{
	int client_fd ;
	client_fd = socket(AF_INET,SOCK_STREAM,0);
	if(client_fd < 0)
		perror("socket error");
	struct sockaddr_in server_sock;
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(12345);
	int err = inet_pton(AF_INET,"127.0.0.1",&server_sock.sin_addr);
	if(err != 1)
		perror("inet_pton error");
	err = connect(client_fd,(struct sockaddr *)&server_sock,sizeof(server_sock));
	if(err == 0)
		printf("connect ok\n");
	send_echo_fun(client_fd);
	close(client_fd);
	return 0;
}
