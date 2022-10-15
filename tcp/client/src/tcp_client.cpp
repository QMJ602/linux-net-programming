#include "tcp_client.h"

Tcp_client::Tcp_client()
{

}

Tcp_client::~Tcp_client()
{
    close(socket_fd);
}

void Tcp_client::Init(char* ip, unsigned short port)
{
    //设置目标服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    //点分字符串转化为无符号整数
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    //创建socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        std::cerr << "Create socket failed" << std::endl;
        exit(-1);
    }
    //建立连接
    int res = connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    if(res < 0)
    {
        std::cerr << "Connect server failed" << std::endl;
        close(socket_fd);
        exit(-1);
    }
}

void Tcp_client::sendstr(char* sendbuf)
{
    size_t len = strlen(sendbuf);
    int send_len = send(socket_fd, sendbuf, len, 0);
    std::cout << "成功发送"  << send_len << "字节" << std::endl;
}