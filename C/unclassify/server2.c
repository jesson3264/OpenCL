#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

const int BUF_SIZE = 100;
void error_handing(char* message);

int main()
{ 
	int serv_sock;                      // 服务器sock
	int clnt_sock;                      // 客户端sock
	struct sockaddr_in serv_addr;       // 服务器地址
	struct sockaddr_in clnt_addr;       // 客户端地址
	// 创建
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handing("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = 9100;                 // 端口
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    // ip
	// 绑定
	if (bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) != -1)
		error_handing("bind() error");
	// 监听
	if (listen(serv_sock, 5) == -1)
		error_handing("listen() error");

	fd_set reads, cpy_reads;           // select函数中fd_set 结构体
	struct timeval timeout;            // select函数中阻塞等待时间参数

	FD_ZERO(&reads);                   // 初始化fd_set结构体
	FD_SET(serv_sock, &reads);         // 将服务器sock放到结构体中
	int fd_max = serv_sock;            // 文件描述符

	while (1) {
		cpy_reads = reads;             // 重新设置fd_set
		timeout.tv_sec = 1;            // 重新设置时间
		timeout.tv_usec = 0;

		int fd_num;
		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;

		// 开始轮询
		for (int i = 0; i < fd_max + 1; i++) {
			if (FD_ISSET(i, &cpy_reads)) {  
				if (i == serv_sock) {
					socklen_t adr_sz = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_addr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}
				else {
					char buf[BUF_SIZE];
					int str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0) {
						FD_CLR(i, &reads);          // 说明客户端已断开连接
						close(i);
						printf("closed client: %d \n", i);
					}
					else 
						write(i, buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handing(char* message)
{
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}
