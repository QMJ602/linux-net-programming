#include "tcp_client.h"

int main()
{
    char* ip = "192.168.131.1";
    unsigned short port = 9000;
    char sendbuf[50];
    Tcp_client tcpc;
    std::cout << "1" << std::endl;
    tcpc.Init(ip, port);
    std::cout << "2" << std::endl;
    while(1)
    {
        std::cout << "请输入发送内容：" << std::endl;
        std::cin >> sendbuf;
        tcpc.sendstr(sendbuf);
    }
    return 0;
}