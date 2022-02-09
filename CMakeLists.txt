cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(server
    src/server.cpp
    include/server.hpp
)

add_executable(client
    src/client.cpp
    include/client.hpp
)

target_link_libraries(server pthread)
target_link_libraries(client pthread)
