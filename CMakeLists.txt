cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++98 -g3")
#^^^ these are the compilation flags for cmake ^^^ can add '-fsanitize=address' for debugging
set(CMAKE_LINKER_FLAGS)

add_executable(webserv testmain.cpp parser.cpp webserv.hpp server.cpp server.hpp)
set_target_properties(webserv PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 98)