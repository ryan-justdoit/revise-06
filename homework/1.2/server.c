#include "common.h"

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <PORT>\n",argv[0]);
		exit(0);
	}

	// 1,创建UDP套接字
	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	// 3,绑定地址
	bind(fd, (struct sockaddr *)&addr, len);

	// 4,使能广播属性
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	// 5,准备好组播地址结构体
	struct sockaddr_in addr2;
	socklen_t len2  = sizeof(addr2);
	bzero(&addr2, len2);

	addr2.sin_family = AF_INET;
	addr2.sin_addr.s_addr = inet_addr("224.0.0,100");
	addr2.sin_port = htons(50003);


	// 6,静静地等待客户端的数据
	char buf[100];
	while(1)
	{
		bzero(buf, 100);
		recvfrom(fd, buf, 100, 0, NULL, NULL);

		printf("收到客户端消息: %s",buf);

		// 转发到组播中
		if(sendto(fd,buf,strlen(buf), 0, (struct sockaddr *)&addr2, len2) == -1)
		{
			perror("sendto failed");
		}

	}

	return 0;
}
