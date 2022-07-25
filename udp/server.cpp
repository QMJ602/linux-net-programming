#include "udp_server.h"
//服务端测试代码
int main()
{
    Udp_server udp_server;
    udp_server.Init();
    while(1)
    {
        udp_server.recieve();
        std::cout << udp_server.re_buf << std::endl;
    }
    return 0;
}