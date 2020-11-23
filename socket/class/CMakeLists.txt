CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

SET(CMAKE_CXX_STANDARD 11)
#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

project(socket_test)

ADD_EXECUTABLE(server_main  server_main.cpp my_server.cpp socket_handler.cpp)
target_link_libraries(server_main pthread)

ADD_EXECUTABLE(client_main  client_main.cpp my_client.cpp socket_handler.cpp)
target_link_libraries(client_main pthread)