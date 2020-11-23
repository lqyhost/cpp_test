#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include<stdio.h>  
#include<stdlib.h>  
#include<netinet/in.h>  
#include<sys/socket.h>  
#include<arpa/inet.h>  
#include<string.h>  
#include<unistd.h>  
#include <thread>

#define BACKLOG 5     //完成三次握手但没有accept的队列的长度  
#define CONCURRENT_MAX 8   //应用层同时可以处理的连接  
#define SERVER_PORT 11332  
#define BUFFER_SIZE 1024  
#define QUIT_CMD ".quit"  


class socket_server
{
public:
    bool run();
    virtual void on_msg(const char* msg){};
    virtual void on_disconnect(int no){};
    void set_client(int no){client_no = no;};
public:
    int client_no = 5;
};

class socket_client
{
public:
    bool start();
private:
    bool is_gateway;
    char name[128];//tora_md tora_td_123456 strategy_test
};

#endif //SOCKET_SERVER_H