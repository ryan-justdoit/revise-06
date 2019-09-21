#ifndef __COMMON_H
#define __COMMON_H

// 基本常用头文件
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

// 全局错误码
#include <errno.h>

// 文件IO操作
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// 数学库
#include <math.h>

// 目录
#include <dirent.h>

// 触摸屏
#include <linux/input.h>

// 液晶屏
#include <sys/mman.h>
#include <linux/fb.h>

// 动态链接动态库
#include <dlfcn.h>

#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/time.h>
#include <sys/ioctl.h>

#include <syslog.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <semaphore.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif
