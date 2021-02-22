cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -Werror -Wextra -fsanitize=address")
#^^^ these are the compilation flags for cmake ^^^ can add '-fsanitize=address' for debugging
set(CMAKE_LINKER_FLAGS)

add_executable(webserv srcs/Response/Response.cpp srcs/Response/Response.hpp srcs/Request/request.cpp srcs/Request/request.hpp srcs/Response/ResponseHeader.cpp srcs/Response/ResponseHeader.hpp
        srcs/Utils/utils.cpp srcs/Utils/utils.hpp srcs/Server/location.cpp srcs/Server/location.hpp srcs/Server/server.cpp srcs/Cluster/serverCluster.cpp
        srcs/Server/parser.cpp srcs/webserv.cpp srcs/webserv.hpp srcs/Response/getPath.cpp)
set_target_properties(webserv PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 98)
