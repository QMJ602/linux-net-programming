#include "udp_client.h"

Udp_client::Udp_client(char* ip, unsigned short port)
{
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);//主机字节序转换为网络字节序
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);//字符串ip转换为无符号整数
}

Udp_client::~Udp_client()
{
    close(socket_fd);
}

void Udp_client::init()
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);//创建UDP套接字
    if(socket_fd < 0)//创建失败
    {
        std::cerr << "Create socket failed" << std::endl;
        exit(-1);
    }
}

unsigned int Udp_client::send(char* sendbuf)
{
    unsigned int str_len = strlen(sendbuf);
    unsigned int len = sendto(socket_fd, sendbuf, str_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    std::cout << "send len=" << len << std::endl;
    return len;
}

void Udp_client::recieve(char* recvbuf)
{
    socklen_t socklen = sizeof(dest_addr);
    recvfrom(socket_fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&dest_addr, &socklen);
}
