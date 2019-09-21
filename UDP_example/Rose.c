#include "common.h"

int main(int argc,char **argv)
{
	// 1,创建套接字
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	// 2,准备跟AF_INET(即IPv4网络层协议)对应的特定IP+PORT地址结构体
	struct sockaddr_in addr;
	socklen_t len =  sizeof(addr);
	bzero(&addr,sizeof(len));

	addr.sin_family = AF_INET;
	//inet_aton("192.168.18.159",&addr.sin_addr); // 绑定指定的ip，并做了字节序转换 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);// 绑定自动获取的IP INADDR_ANY位二进制数据没有做字节序转换
	addr.sin_port   =htons(50525);

	// 3,绑定地址
	bind(fd, (struct sockaddr *)&addr,len);
	
	// 4,准备接受对放的地址信息
	struct sockaddr_in peeraddr;
	len = sizeof(peeraddr);
	bzero(&peeraddr,len);

	// 5,静静地等待对方的信件..
	char buf[100];
	while(1)
	{
		bzero(buf,100);
		recvfrom(fd, buf, 100, 0, (struct sockaddr *)&peeraddr, &len);
		
		printf("收到 [%s:%hu] 的信息:%s",
				inet_ntoa(peeraddr.sin_addr),
				ntohs(peeraddr.sin_port),
				buf);
	}

	return 0;
}
