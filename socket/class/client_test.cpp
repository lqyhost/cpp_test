#include <iostream>
#include "my_client.h"

void add_connect()
{
    my_client m;
    std::thread socket_server_thread(&my_client::start, &m);
    socket_server_thread.join();
}

int main()
{
    for (int i =0;i<10;i++)
    {
        std::cout<<"i:"<<i<<std::endl;
        add_connect();
        // my_client m;
        // std::thread socket_server_thread(&my_client::start, &m);
        // socket_server_thread.detach();
    }

    return 0;
}