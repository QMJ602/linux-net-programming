#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

class Udp_client{
    public:
        Udp_client(char* ip, unsigned short port);
        ~Udp_client();
        void init();
        unsigned int send(char* sendbuf);
    private:
        struct sockaddr_in dest_addr;//目标地址结构体
        int socket_fd;
        
};