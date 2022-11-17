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
#include <unordered_map>
#include <queue>

//客户数据：客户地址，要发送给客户的数据队列，从客户端读入的数据
struct client_data{
    sockaddr_in address;
    std::queue<std::string> write_queue;
    char rev_buf[64];
};


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
 * 功能：向epoll事件表注册/修改/删除文件描述符fd的事件
 * epollfd：epoll文件描述符
 * fd：要注册的文件描述符
 * EVT：要注册的事件
 * set_ET：是否设为边缘触发模式
 * set_nonblock：是否将fd设置为非阻塞模式
 * ***********************************************************/
void addfd(int epollfd, int fd, int option, unsigned int EVT, bool set_ET, bool set_nonblock)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EVT;
    if(set_ET)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, option, fd, &event);
    if(set_nonblock)
    {
        setnonblocking(fd);
    }
}

//传入本机的ip,端口
int main(int argc, char* argv[])
{
    if(argc <= 2)//参数不全
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    const char* port = argv[2];

    sockaddr_in host_addr;
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &host_addr.sin_addr);

    //创建监听sockfd
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listen_fd >= 0);

    //绑定本机ip和端口
    int ret = bind(listen_fd, (sockaddr*)&host_addr, sizeof(host_addr));
    assert(ret != -1);
    //设为监听fd
    ret = listen(listen_fd, 5);
    std::cout << "listen:" << std::endl;
    //创建epollfd
    int epollfd = epoll_create(10);
    //注册listenfd的可读事件
    addfd(epollfd, listen_fd, EPOLL_CTL_ADD, EPOLLIN, false, false);

    //哈希表存储客户数据信息
    std::unordered_map<int, client_data> client_map;

    epoll_event events[10];//设置最多存放10个事件
    while(1)
    {
        ret = epoll_wait(epollfd, events, 10, -1);//-1阻塞
        if(ret < 0)
        {
            std::cerr << "epoll failure." << std::endl;
        }
        for(int i=0; i<ret; ++i)
        {
            if(events[i].data.fd == listen_fd && (events[i].events & EPOLLIN))//监听到新的连接
            {
                client_data client;//创建一个新的用户数据
                socklen_t client_addrlen = sizeof(client.address);
                int connfd = accept(listen_fd, (sockaddr*)&client.address, &client_addrlen);
                assert(connfd >= 0);
                std::cout << "A client entered." << std::endl;
                //将用户信息加入map中
                client_map[connfd] = client;
                //注册新连接fd的可读和EPOLLRDHUP事件  在对端正常断开连接时，会触发EPOLLIN | EPOLLRDHUP事件，因此可以用EPOLLRDHUP来判断对方是否断开连接
                addfd(epollfd, connfd, EPOLL_CTL_ADD, (EPOLLIN|EPOLLRDHUP), false, false);
            }
            //对端断开连接  放在EPOLLIN事件处理之前
            else if(events[i].events & EPOLLRDHUP)
            {
                client_map.erase(events[i].data.fd);//删除用户数据
                close(events[i].data.fd);//关闭服务端fd
                std::cout << "A client left." << std::endl;
            }
            //可读
            else if(events[i].events & EPOLLIN)
            {
                int fd = events[i].data.fd;
                memset(client_map[fd].rev_buf, 0, sizeof(client_map[fd].rev_buf));
                int rev_len = recv(fd, client_map[fd].rev_buf, sizeof(client_map[fd].rev_buf), 0);
                if(rev_len <= 0)
                {
                    close(fd);
                }
                printf("get %d bytes from %d.\n", rev_len, fd);
                for(auto& client_pr : client_map)
                {
                    if(client_pr.first == fd)
                        continue;
                    //将接收到的消息加入其他用户的消息发送队列
                    client_pr.second.write_queue.emplace(std::string(client_map[fd].rev_buf));
                    //在注册表中添加fd的可写事件
                    addfd(epollfd, client_pr.first, EPOLL_CTL_MOD, (EPOLLIN|EPOLLRDHUP|EPOLLOUT), false, false);
                }
            }
            //如果注册了EPOLLOUT，在LT模式下，只要可写（发送缓冲区不满）就会一直触发
            else if(events[i].events & EPOLLOUT)
            {
                int fd = events[i].data.fd;
                //发送队列为空，没有数据要发送
                if(client_map[fd].write_queue.empty())
                {
                    //关闭该fd的可写事件
                    addfd(epollfd, fd, EPOLL_CTL_MOD, (EPOLLIN|EPOLLRDHUP), false, false);
                    continue;
                }
                char sendbuf[64];
                //从队列中取出一条消息发送    .c_str返回的是一个临时指针，不能对其操作，需复制到新的内存区域。
                strcpy(sendbuf, client_map[fd].write_queue.front().c_str());
                client_map[fd].write_queue.pop();
                ret = send(fd, sendbuf, strlen(sendbuf), 0);
            }
        }
    }
    return 0;
}