#include "common.h"

int fd;

void usage(char *argv)
{
	printf("Usage: %s <IP> <PORT>\n",argv);
	exit(0);
}

void f(int sig)
{
	// 准备接受对方的地址信息
	struct sockaddr_in peeraddr;
	socklen_t len = sizeof(peeraddr);
	bzero(&peeraddr,len);

	char buf[100];
	bzero(buf,100);
	recvfrom(fd, buf, 100, 0, (struct sockaddr *)&peeraddr, &len);

	printf("收到 [%s:%hu] 的信息:%s",
			inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port),
			buf);
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		usage(argv[0]);
	}

	// 1,创建套接字
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 2,准备地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr,len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[2]));

	// 3.绑定地址
	bind(fd, (struct sockaddr *)&addr, len);

	// 4.加入指定的多播组
	struct ip_mreq m;
	bzero(&m, sizeof(m));

	//inet_aton("244.0.0.100", &m.imr_multiaddr);
	//inet_atom("192.168.18.159", &m.imr_interface);

	m.imr_multiaddr.s_addr = inet_addr(argv[1]);
	//m.imr_interface.s_addr = inet_addr("192.168.18.159");
	m.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &m, sizeof(m));

	// 5.用信号的方式来异步接受各个客户发来的信息
	 
	// a.捕捉信号
	signal(SIGIO,f);

	// b.设置套接字为异步工作模式(即使之收到数据产生信号SIGIO)
	long flag = fcntl(fd, F_GETFL);
	flag |= O_ASYNC;
	fcntl(fd, F_SETFL, flag);

	// c.指定本进程为信号的属主
	fcntl(fd, F_SETOWN,getpid());

	// 服务器忙别的事情
	int i = 0;
	while(1)
	{
		printf("%d\n", ++i);
		sleep(1);
	}

	return 0;
}
