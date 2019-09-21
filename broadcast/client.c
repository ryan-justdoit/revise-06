#include "common.h"

void usage(char *argv)
{
	printf("Usage: %s <PROT>\n",argv);
	exit(0);
}

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		usage(argv[0]);
	}

	// 1,创建套接字
	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	inet_aton("192.168.18.255", &addr.sin_addr);
	addr.sin_port = htons(atoi(argv[1]));

	// 3,使能广播属性
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	// 4,广播消息
	char *msg = "this is a broadcast massage...";
	while(1)
	{
		sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&addr, len);
		sleep(1);
	}


	return 0;
}
