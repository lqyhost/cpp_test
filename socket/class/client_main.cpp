#include <iostream>
#include "my_client.h"

int main()
{
    my_client m;
    std::thread socket_server_thread(&my_client::start, &m);
    socket_server_thread.join();
    return 0;
}