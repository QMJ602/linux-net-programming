#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

class Tcp_client{
public:
    Tcp_client();
    ~Tcp_client();
    void Init(char* ip, unsigned short port);
    void sendstr(char* sendbuf);
private:
    struct sockaddr_in server_addr;
    int socket_fd;
};