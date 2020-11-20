CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
#SET(CMAKE_CXX_FLAGS -pthread)

project(socket_test)

ADD_EXECUTABLE(server_1  socket_server_1.cpp)
ADD_EXECUTABLE(client_1  socket_client_1.cpp)

ADD_EXECUTABLE(server_2  socket_server_2.cpp)
ADD_EXECUTABLE(client_2  socket_client_2.cpp)

ADD_EXECUTABLE(server_3  socket_server_3.cpp)
ADD_EXECUTABLE(client_3  socket_client_3.cpp)
ADD_EXECUTABLE(client_4  socket_client_3.cpp)