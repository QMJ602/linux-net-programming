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

/**************************************************************
 * 功能：向epoll事件表注册文件描述符fd的EPOLLIN事件
 * epollfd：epoll文件描述符
 * fd：要注册的文件描述符
 * set_ET：是否设为边缘触发模式
 * set_nonblock：是否将fd设置为非阻塞模式
 * ***********************************************************/
void addfd(int epollfd, int fd, bool set_ET, bool set_nonblock)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(set_ET)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    if(set_nonblock)
    {
        setnonblocking(fd);
    }
}

/**************************************************************
 * 功能：水平触发模式下的处理流程
 * epollfd：epoll文件描述符
 * stdin_fd：标准输入的文件描述符
 * ***********************************************************/
void LT_mode(int epollfd, int stdin_fd, int sockfd, const char* ip, const char* port)
{
    epoll_event events[10];//存放就绪的事件
    char recv_buf[100];//接收缓冲区

    //创建管道
    int pipe_fd[2];
    int ret = pipe(pipe_fd);
    assert(ret == 0);
    
    while(1)
    {
        std::cout << "输入发送内容：" << std::endl;
        int ret = epoll_wait(epollfd, events, 10, -1);
        if(ret < 0)
        {
            std::cerr << "epoll failure." << std::endl;
        }
        for(int i=0;i<ret;++i)
        {
            //标准输入事件
            if(events[i].data.fd == stdin_fd)
            {
                //从标准输入转移到管道(splice至少有一方是管道，所以这里以管道作中介)
                splice(stdin_fd, NULL, pipe_fd[1], NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);
                //从管道转移到sockfd
                splice(pipe_fd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);
            }
            //服务器发来数据   设置为非边缘触发时，只要内核接收缓冲区中还有数据，sockfd的EPOLLIN事件就会在每次调用epoll_wait时都触发
            else if(events[i].data.fd == sockfd && (events[i].events & EPOLLIN))
            {
                memset(recv_buf, 0, sizeof(recv_buf));
                int rec_len = recv(sockfd, recv_buf, sizeof(recv_buf), 0);
                if(rec_len > 0)
                {
                    printf("——————ip:%s port:%s ——————：\n%s\n", ip, port, recv_buf);
                }
                else if(rec_len <= 0)//返回值为0表示连接关闭,<0表示异常
                {
                    close(sockfd);
                }
            }
        }
    }
}

/**************************************************************
 * 功能：边缘触发模式下的处理流程
 * epollfd：epoll文件描述符
 * stdin_fd：标准输入的文件描述符
 * ***********************************************************/
void ET_mode(int epollfd, int stdin_fd, int sockfd, const char* ip, const char* port)
{
    epoll_event events[10];//存放就绪的事件
    char recv_buf[100];//接收缓冲区

    //创建管道
    int pipe_fd[2];
    int ret = pipe(pipe_fd);
    assert(ret == 0);
    
    while(1)
    {
        std::cout << "输入发送内容：" << std::endl;
        int ret = epoll_wait(epollfd, events, 10, -1);
        if(ret < 0)
        {
            std::cerr << "epoll failure." << std::endl;
        }
        for(int i=0;i<ret;++i)
        {
            //标准输入事件
            if(events[i].data.fd == stdin_fd)
            {
                //从标准输入转移到管道(splice至少有一方是管道，所以这里以管道作中介)
                splice(stdin_fd, NULL, pipe_fd[1], NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);
                //从管道转移到sockfd
                splice(pipe_fd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);
            }
            //服务器发来数据   设置为边缘触发时，需要一次性读完缓冲区中的数据
            else if(events[i].data.fd == sockfd && (events[i].events & EPOLLIN))
            {
                while(1)
                {
                    memset(recv_buf, 0, sizeof(recv_buf));
                    int rec_len = recv(sockfd, recv_buf, sizeof(recv_buf), 0);//sockfd必须是非阻塞的。否则会阻塞在最后一次读取
                    if(rec_len > 0)
                    {
                        printf("\n——————ip:%s port:%s ——————：\n%s\n", ip, port, recv_buf);
                    }
                    else if(rec_len == 0)//返回值为0表示连接关闭
                    {
                        close(sockfd);
                    }
                    else if(rec_len < 0)//异常
                    {
                        if(errno == EAGAIN || errno == EWOULDBLOCK)//没有数据可读
                        {
                            std::cout << "last_read" << std::endl;
                            break;
                        }
                    }
                }
            }
        }
    }
}

//传入目标服务器ip和端口
int main(int argc, char* argv[])
{
    if(argc <= 2)//参数不全
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    const char* port = argv[2];

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

    int epollfd = epoll_create(2);
    int stdin_fd = 0;//标准输入的文件描述符
    
    // //注册标准输入fd和sockfd
    addfd(epollfd, stdin_fd, false, true);//水平触发，非阻塞
    addfd(epollfd, sockfd, false, true);//设置为非边缘触发，非阻塞

    LT_mode(epollfd, stdin_fd, sockfd, ip, port);

    //注册标准输入fd和sockfd
    // addfd(epollfd, stdin_fd, false, true);//水平触发，非阻塞
    // addfd(epollfd, sockfd, true, true);//设置为边缘触发，非阻塞

    // ET_mode(epollfd, stdin_fd, sockfd, ip, port);

    return 0;
}

