#include "tcp_client.h"

int main()
{
    char* ip = "192.168.230.132";
    unsigned short port = 9000;
    char sendbuf[50];
    Tcp_client tcpc;
    tcpc.Init(ip, port);
    while(1)
    {
        std::cout << "请输入发送内容：" << std::endl;
        std::cin >> sendbuf;
        tcpc.sendstr(sendbuf);
    }
    return 0;
}