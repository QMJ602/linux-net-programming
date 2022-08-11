#include "tcp_server.h"

Tcp_server::Tcp_server()
{

}

Tcp_server::~Tcp_server()
{
    close(socket_fd);
}

void Tcp_server::Init()
{
    //设置本机地址
    unsigned short port = 8000;
    char* ip = "192.168.230.1";

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建套接字
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        std::cerr << "Create socket failed" << std::endl;
        exit(-1);
    }

    //绑定套接字和本机地址
    int ret = bind(socket_fd, (struct sockaddr*)&myaddr, sizeof(myaddr));
    if(ret < 0)
    {
        std::cerr << "Bind ip failed" << std::endl;
        exit(-1); 
    }

    //监听连接的到来  将套接字由主动修改为被动，使操作系统为该套接字设置一个连接队列，用来记录所有连接到该套接字的连接。
    listen(socket_fd, 10); //套接字， 连接队列的长度 

}

