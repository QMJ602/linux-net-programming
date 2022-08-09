#include "udp_server.h"

Udp_server::Udp_server()
{
    unsigned short port = 8000;//设置本机端口
    myaddr.sin_family = AF_INET;//域
    myaddr.sin_port = htons(port);//主机字节序转换为网络字节序
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本机网卡的所有ip地址
}

Udp_server::~Udp_server()
{
    close(socket_fd);
}

void Udp_server::Init()
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);//创建socket
    if(socket_fd < 0)
    {
        perror("create socket failed");
        exit(-1);
    }
    int bd = bind(socket_fd, (struct sockaddr*)&myaddr, sizeof(myaddr));//将本地固定ip端口和socket绑定
    if(bd < 0)
    {
        perror("bind add failed");
        exit(-1);
    }
}

void Udp_server::recieve()
{
    memset(re_buf, 0, sizeof(re_buf));//清空接收缓冲区
    socklen_t addrlen = sizeof(myaddr);
    unsigned int len = recvfrom(socket_fd, re_buf, sizeof(re_buf), 0, (struct sockaddr*)&myaddr, &addrlen);
    std::cout << "recieved len=" << len << std::endl;
}