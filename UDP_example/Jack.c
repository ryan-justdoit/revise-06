#include "common.h"

void usage(char *argv)
{
	printf("Usage: %d <IP> <PORT>\n",argv[0]);
	exit(0);
}


int main(int argc,char **argv)
{
	if(argc != 3)
	{
		usage(argv[0]);
	}

	// 1,(邮箱)创建套接字
	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 3,(对方的地址)准备对方的IP PORT
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, sizeof(len));
	
	addr.sin_family = AF_INET;
	inet_aton(argv[1], &addr.sin_addr);
	addr.sin_port = htons(atoi(argv[2]));

	char buf[100];
	while(1)
	{
		bzero(buf,100);
		fgets(buf,100,stdin);
		sendto(fd, buf, 100, 0, (struct sockaddr *)&addr, len);
	}

	return 0;
}
