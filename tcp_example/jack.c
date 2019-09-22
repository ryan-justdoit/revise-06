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
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	inet_aton(argv[1], &addr.sin_addr);
	addr.sin_port = htons(atoi(argv[2]));

	// 3,链接对方
	connect(fd, (struct sockaddr *)&addr, len);

	// 4,给对方发数据
	char msg[100];
	while(1)
	{
		bzero(msg,100);
		fgets(msg, 100, stdin);
		if(write(fd, msg, 100) == 0)
			break;
	}	
	close(fd);

	return 0;
}
