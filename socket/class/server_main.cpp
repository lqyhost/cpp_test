#include <iostream>
#include "my_server.h"

int main()
{
    my_server m;
    std::thread socket_server_thread(&my_server::start, &m);
    socket_server_thread.join();
    return 0;
}