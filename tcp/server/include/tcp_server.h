#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

class Tcp_server{
public:
    Tcp_server();
    ~Tcp_server();
    void Init();
private:
    int socket_fd;
    struct sockaddr_in myaddr;
};