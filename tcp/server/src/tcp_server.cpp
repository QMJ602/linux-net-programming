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
    unsigned short port = 9000;

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
    std::cout << "listen" << std::endl;
    while(1)
    {
        struct sockaddr_in client_addr;
        int connfd;
        socklen_t addr_len = sizeof(client_addr);
        //取出已连接的套接字
        connfd = accept(socket_fd, (struct sockaddr*)&client_addr, &addr_len);
        std::cout << "accept" << std::endl;
        if(connfd < 0)
        {
            std::cerr << "accept" << std::endl;
            continue; 
        }
        char client_ip[INET_ADDRSTRLEN];
        //ip地址转换为点分10进制字符串
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        unsigned short client_port = ntohs(client_addr.sin_port);

        printf("client ip=%s, port=%d\n", client_ip, client_port);

        char recv_buf[100];
        memset(recv_buf, 0, sizeof(recv_buf));
        while(recv(connfd, recv_buf, sizeof(recv_buf), 0) > 0)
        {
            printf("接收到的数据：%s\n", recv_buf);
            send(connfd, recv_buf, sizeof(recv_buf), 0);//回射
            memset(recv_buf, 0, sizeof(recv_buf));
        }
        memset(recv_buf, 0, sizeof(recv_buf));
        close(connfd);
    }
    close(socket_fd);
}

