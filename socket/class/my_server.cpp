#include <iostream>
#include "my_server.h"

bool my_server::run()
{
    std::cout<<"run call"<<std::endl;
    socket_server::set_client(100);
    socket_server::run();
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