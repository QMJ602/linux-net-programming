#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

class Udp_server{
    public:
        Udp_server();
        ~Udp_server();
        void Init();
        void recieve();
        char re_buf[50];
    private:
        struct sockaddr_in myaddr;//本机地址结构体
        int socket_fd;
        
};