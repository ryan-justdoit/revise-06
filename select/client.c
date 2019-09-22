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
	// 1,create socket
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	// 2,prepare to the struct of address
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));
	
	// 3,connect
	if(connect(fd, (struct sockaddr *)&addr, len) != 0)
	{
		perror("connect failed");
		exit(0);
	}	
	
	int maxfd = fd > STDIN_FILENO ? fd: STDIN_FILENO;

	fd_set rset; // 读就绪描述符集合
	fd_set wset; // 写就绪描述符集合
	fd_set eset; // 异常就绪描述符集合

	char buf[100];
	while(1)
	{
		// 清空集合
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_ZERO(&eset);

		// 将描述符放入对应就绪状态的集合中
		// 由于要关注什么时候能读取键盘数据，因此放入rset
		// 由于要关注什么时候能读取对端数据，因此也放入rset
		FD_SET(STDIN_FILENO, &rset);
		FD_SET(fd, &rset);

		// maxfd:三个集合中所有描述符的最大值
		// NULL :时间结构体，用来控制select的阻塞时长
		// 	 指定为NULL代表永久阻塞
		select(maxfd+1, &rset, &wset,&eset, NULL);

		// select函数返回之后，三个描述符集合会被重新设定
		// 设定规则：
		// 	凡是处于就绪状态的描述符，留下
		// 	凡是未处于就绪状态的描述，删除

		// 接下来就可以通过判断各个描述符是否残留在集合中
		// 来进行相应的操作了

		// 键盘有输入了
		if(FD_ISSET(STDIN_FILENO, &rset))
		{
			bzero(buf, 100);
			fgets(buf, 100, stdin);

			write(fd, buf, strlen(buf));
		}

		// 服务器有数据来了
		if(FD_ISSET(fd, &rset))
		{
			bzero(buf, 100);
			read(fd, buf, 100);
			printf("来自服务端: %s",buf);
		}
	}
	

	return 0;
}
