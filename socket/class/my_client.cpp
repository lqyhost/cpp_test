#include <iostream>
#include "my_client.h"

bool my_client::start()
{
    std::cout<<"start call"<<std::endl;
    socket_client::start();
    return true;
}