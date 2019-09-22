#include "common.h"

char *uppercase(char *msg)
{	
	int step = 'a' - 'A';
	static char s[200];

	for(int i=0; i<strlen(msg); i++)
	{
		if(msg[i] >= 'a' && msg[i] <= 'z')
		{
			s[i] = msg[i]-step;
			continue;
		}
		s[i] = msg[i];
		usleep(20*1000);

	}
	return s;

}


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
	// 1,创建套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr,len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	// 3,绑定地址
	if(bind(fd, (struct sockaddr *)&addr, len) != 0)
	{
		perror("bind failed");
		exit(0);
	}

	// 4,设定监听状态
	// 注意：
	// 	只有处于监听状态的TCP套接字，才能接受对端的链接请求
	
	listen(fd, 3);

	// 5,静静地等待对端的链接..
	struct sockaddr_in peeraddr;
	len = sizeof(peeraddr);
	bzero(&peeraddr, len);

	int connfd = accept(fd, (struct sockaddr *)&peeraddr, &len);

	printf("连接成功，对端地址:%s: %hu\n",
			inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port));

	char msg[100];
	while(1)
	{
		bzero(msg, 100);
		if(read(connfd, msg, 100) == 0)
		{
			break;
		}
		char *s = uppercase(msg);
		write(connfd,s,strlen(s));
	}

	printf("客户端已经断开了连接\n");
	close(connfd);
	close(fd);


	return 0;
}
