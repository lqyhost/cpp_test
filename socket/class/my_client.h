#ifndef MY_SERVER_H
#define MY_SERVER_H

#include "socket_handler.h"

class my_client: public socket_client
{
public:
    bool start();
};

#endif //SOCKET_SERVER_H