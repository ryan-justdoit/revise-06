#include "common.h"

int fd;

void f(int sig)
{
	// 准备保准服务端的地址信息
	struct sockaddr_in peeraddr;
	socklen_t len = sizeof(peeraddr);
	bzero(&peeraddr, len);

	char buf[100];
	bzero(buf, 100);
	printf("等待服务器的消息...\n");
	recvfrom(fd, buf, 100, 0, (struct sockaddr *)&peeraddr, &len);

	printf("收到来自服务端[%s:%hu]主播的信息: %s",
			inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port),
			buf);
}

void usage(char *argv)
{
	printf("Usage: %s <Server-IP> <PORT>\n",argv);
	exit(0);
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		usage(argv[0]);
	}

	// 1,创建套接字
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 2,准备对端服务器的地址结构体
	struct sockaddr_in myaddr;
	socklen_t mylen = sizeof(myaddr);
	bzero(&myaddr, mylen);

	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(50003);

	// 3,绑定固定的地址，方便服务器主动给我发数据
	bind(fd, (struct sockaddr *)&myaddr, mylen);

	// 4,加入多播组
	struct ip_mreq m;
	bzero(&m, sizeof(m));

	m.imr_multiaddr.s_addr = inet_addr("224.0.0.100");
	m.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &m, sizeof(m));

	// a.捕捉信号
	signal(SIGIO,f);

	// b.设置套接字为异步工作模式(即使之收到数据是产生信号SIGIO)
	long flag = fcntl(fd, F_GETFL);
	flag |= O_ASYNC;
	fcntl(fd, F_SETFL, flag);

	// c.指定本进程为信号的属主
	fcntl(fd, F_SETOWN, getpid());

	// 5,准备服务器的地址结构体
	struct sockaddr_in addr;
	socklen_t len  = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr =inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));

	// 6,不断给服务器发送数据
	// 　当收到服务器发来的组播消息时，会触发SIGIO
	//　 继而会使fgets()出错返回

	char buf[100];
	while(1)
	{
		bzero(buf, 100);
		if(fgets(buf, 100, stdin) == NULL)
		{
			perror("fgets failed");
			continue;
		}

		sendto(fd, buf, strlen(buf), 0,
			       	(struct sockaddr *)&addr, len);
	}

	return 0;
}
