#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include<sys/sendfile.h>

// static const int READ_BUFFER_SIZE = 2048;
// static const int WRITE_BUFFER_SIZE = 1024;
class Http_conn{
public:
    static const int READ_BUFFER_SIZE  = 4096;
    static const int WRITE_BUFFER_SIZE = 1024;

    //请求方法
    enum METHOD{
        GET = 0, HEAD, POST, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH
    };
    //主状态机状态  （正在分析）请求行/请求头/请求体
    enum CHECK_STATE{
        CHECK_STATE_REQUEST_LINE = 0, CHECK_STATE_REQUEST_HEAD, CHECK_STATE_REQUEST_CONTENT
    };
    //从状态机状态  （当前行的状态）读到一个完整行/行出错/行尚不完整
    enum LINE_STATUS{
        LINE_OK = 0, LINE_BAD, LINE_OPEN
    };
    //处理http请求的结果  请求不完整/获得完整请求/请求有语法错误
    enum HTTP_CODE{
        NO_REQUEST = 0, GET_REQUEST, BAD_REQUEST, INTERNAL_ERROR, FORBIDDEN_REQUEST, NO_RESOURCE, FILE_REQUEST
    };
public:
    Http_conn(int sockfd, sockaddr_in client_address);
    ~Http_conn();
public:
    void process();
    void init();
    bool read();
    bool write();
private:
    LINE_STATUS parse_line();
    HTTP_CODE parse_request_line(char* temp);
    HTTP_CODE parse_request_head(char* temp);
    HTTP_CODE parse_request_content(char* temp);
    HTTP_CODE process_read();
    HTTP_CODE do_request();
    bool add_response(std::string text);
    bool add_status_line(int status, const char* title);
    bool add_linger();
    bool add_blankline();
    bool add_content_length(int content_lenth);
    bool add_headers(int content_lenth);
    bool add_content(const char* content);
    bool process_write(HTTP_CODE read_ret);
public:
    //是否保持连接
    bool m_linger;

private:
    //读缓冲区
    char m_readbuf[READ_BUFFER_SIZE];
    //发送缓冲区
    char m_sendbuf[WRITE_BUFFER_SIZE];
    //请求正文长度
    int m_content_length;
    //请求的主机名
    char* m_host;

    //请求方法
    METHOD m_method;
    //请求资源路径
    char* m_url;
    //版本
    char* m_version;
    //请求正文
    char* m_content;
    //请求的目标文件路径
    char* m_target_file;
    //要发送的文件描述符
    int m_file_fd;

    //stat函数返回的文件属性
    struct stat m_file_stat;
    int m_read_index;
    int m_check_index;
    
    CHECK_STATE m_check_state;
    int m_startline;
    sockaddr_in m_client_address;
    int m_sockfd;
};
#endif