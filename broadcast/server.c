#include "common.h"

int fd;
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
	
	// 1,创建一个套接字
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	bind(fd, (struct sockaddr *)&addr, len);

	char msg[100];
	
	// 3,准备发送方的地址结构体
	struct sockaddr_in peeraddr;
	len = sizeof(peeraddr);
	bzero(&peeraddr,len);

	while(1)
	{
		bzero(msg,100);
		recvfrom(fd, msg, 100, 0, (struct sockaddr *)&peeraddr, &len);
		printf("收到 [%s:%hu] 的消息 %s\n",
			inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port),
			msg);
	}

	return 0;
}
