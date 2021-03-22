cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -Werror -Wextra -fsanitize=address")
#^^^ these are the compilation flags for cmake ^^^ can add '-fsanitize=address' for debugging
set(CMAKE_LINKER_FLAGS)

add_executable(
		# executable name
		webserv

		# CGI
		srcs/CGI/CGI.cpp
		srcs/CGI/CGI.hpp

		# Cluster
		srcs/Cluster/serverCluster.cpp
		srcs/Cluster/serverCluster.hpp

		# Request
		srcs/Request/request.cpp
		srcs/Request/request.hpp

		# Response
		srcs/Response/getPath.cpp
		srcs/Response/getPath.hpp
		srcs/Response/Response.cpp
		srcs/Response/Response.hpp
		srcs/Response/responseHeader.cpp
		srcs/Response/responseHeader.hpp

		# Server
		srcs/Server/connection.cpp
		srcs/Server/connection.hpp
		srcs/Server/location.cpp
		srcs/Server/location.hpp
		srcs/Server/parser.cpp
		srcs/Server/server.hpp
		srcs/Server/serverBasic.cpp
		srcs/Server/serverUtility.cpp

		# Utils
		srcs/Utils/utils.cpp
		srcs/Utils/utils.hpp
		srcs/Utils/Base64.cpp
		srcs/Utils/Base64.hpp

		# main
		srcs/webserv.cpp
		srcs/webserv.hpp
		srcs/Utils/defines.hpp)
set_target_properties(webserv PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 98)
