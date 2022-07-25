#include "udp_client.h"
//客户端测试代码
int main()
{
    char* ip = "192.168.230.132";
    unsigned short port = 8000;
    Udp_client udpc(ip, port);
    udpc.init();
    char sendbuf[50];
    while(1)
    {
        std::cout << "输入发送内容：" << std::endl;
        std::cin >> sendbuf;
        udpc.send(sendbuf);
        // memset(sendbuf, 0, sizeof(sendbuf));
    }
    return 0;
}