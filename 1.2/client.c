#include "common.h"

int fd;

void usage(char *argv)
{
	printf("Usage: %s <IP> <PORT>\n",argv);
	exit(0);
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		usage(argv[0]);
	}
	// 1,创建UDP套接字
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	// 2,准备服务器的IP+PORT地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	inet_aton(argv[1], &addr.sin_addr);
	addr.sin_port = htons(atoi(argv[2]));
	
	// 3,给服务器发送消息
	char buf[100];
	while(1)
	{
		bzero(buf,100);
		fgets(buf,100,stdin);

		sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&addr, len);
	}


	return 0;
}
