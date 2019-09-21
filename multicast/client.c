#include "common.h"

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

	// 1,创建套接字
	int fd =socket(AF_INET, SOCK_DGRAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	inet_aton(argv[1], &addr.sin_addr); // 准备好对方的ip
	addr.sin_port = htons(atoi(argv[2])); // 对方的端口

	// 使能广播属性
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on ,sizeof(on));

	// 不断发送数据
	char *msg = "只是广播/主播/单播";
	while(1)
	{
		sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&addr, len);
		sleep(1);
	}


	return 0;
}
