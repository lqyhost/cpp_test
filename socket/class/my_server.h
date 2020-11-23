#ifndef MY_SERVER_H
#define MY_SERVER_H

#include "socket_handler.h"


class my_server: public socket_server
{
public:
    bool start();
    void on_msg(const char* msg)override;
    void on_disconnect(int no)override;
};

#endif //SOCKET_SERVER_H