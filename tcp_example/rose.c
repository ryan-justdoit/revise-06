#include "common.h"

void usage(char *argv)
{
	printf("Usage: %s <PORT>\n",argv);
	exit(0);
}

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		usage(argv[0]);
	}

	// 1,创建tcp套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr,len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	// 3,绑定地址到指定的通信端点
	bind(fd, (struct sockaddr *)&addr, len);

	// 4,设定监听状态
	// 注意：
	// 	只有处于监听状态的TCP套接字，才能接受对端的链接请求
	// 	参数3规定的是通信端点能同时接受连接请求的数量
	// 	注意不是链接请求的总数
	listen(fd, 3);

	// 5,静静地等待对端的链接...
	struct sockaddr_in peeraddr;
	len = sizeof(peeraddr);
	bzero(&peeraddr,len);
	
	int connfd = accept(fd, (struct sockaddr *)&peeraddr, &len);

	printf("连接成功，对端地址: %s:%hu\n",
			inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port));

	char msg[100];
	while(1)
	{
		bzero(msg,100);
		if(read(connfd, msg, 100) == 0)
			break;

		printf("%s",msg);
	}

	printf("客户端已经断开了连接\n");
	close(connfd);
	close(fd);

	return 0;
}
