//测试一下当send发送的数据大小大于内核发送缓冲区大小时会发生什么
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

/**************************************************************
 * 功能：通过fcntl()设置文件描述符为非阻塞模式
 * fd：要设置的文件描述符
 * ***********************************************************/
int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

int main()
{
    const char* ip = "192.168.230.132";
    const char* port = "8800";
    sockaddr_in target_addr;//目标服务器地址结构体
    target_addr.sin_family = AF_INET;//域
    target_addr.sin_port = htons(atoi(port));//端口->网络字节序
    inet_pton(AF_INET, ip, &target_addr.sin_addr);//ip  点分字符串->32位无符号整数->网络字节序
    //创建socket  
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    //连接服务器
    if(connect(sockfd, (sockaddr*)&target_addr, sizeof(target_addr)) < 0)
    {
        std::cerr << "connection falied." << std::endl;
        exit(-1);
    }
    //设置为非阻塞
    setnonblocking(sockfd);
    int sendbufLen = 2048;
    setsockopt(sockfd,SOL_SOCKET, SO_SNDBUF,(const char*)&sendbufLen, sizeof(int));
    int tmp;
    socklen_t optlen = sizeof(int); 
    getsockopt(sockfd,SOL_SOCKET, SO_SNDBUF,(int *)&tmp, &optlen); 
    printf("send_tmp=%d,optlen=%d\n",tmp,(int)optlen);
    char* sendbuf = (char*)malloc(10000);
    int sendlen = send(sockfd, sendbuf, 10000, 0);
    printf("sendlen=%d\n", sendlen);
    return 0;
}