#include <iostream>
#include "my_server.h"

bool my_server::start()
{
    std::cout<<"start call"<<std::endl;
    socket_server::start();
    return true;
}

void my_server::on_msg(const char* msg)
{
    std::cout<<"on_msg:"<<msg<<std::endl;
}

void my_server::on_disconnect(int no)
{
    std::cout<<"on_disconnect:"<<no<<std::endl;
}