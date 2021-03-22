# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: thvan-de <thvan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/07/25 17:28:30 by pde-bakk      #+#    #+#                  #
#    Updated: 2021/01/25 10:59:18 by thvan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = webserv

ifdef COMPILER
 CXX = $(COMPILER)
endif
_INCLUDE = CGI/CGI.hpp
		   Cluster/serverCluster.hpp
		   Request/request.hpp
		   Response/getPath.hpp
		   Response/Response.hpp
		   Response/responseHeader.hpp
		   Server/connection.hpp
		   Server/location.hpp
		   Server/server.hpp
		   Utils/Base64.hpp
		   Utils/defines.hpp
		   Utils/utils.hpp
		   webserv.hpp
_SRCS =	   CGI/CGI.cpp
		   Cluster/serverCluster.cpp
		   Request/request.cpp
		   Response/getPath.cpp
		   Response/Response.cpp
		   Response/responseHeader.cpp
		   Server/connection.cpp
		   Server/location.cpp
		   Server/serverBasic.cpp
		   Server/serverUtility.cpp
		   Utils/Base64.cpp
		   Utils/utils.cpp
		   webserv.cpp

CXXFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c++98

SRCS = $(addprefix srcs/, $(_SRCS))

INCLUDES = $(addprefix srcs/, $(_INCLUDE))


all: $(NAME)

fclean: clean
	@rm -rf $(NAME) $(NAME)

re: fclean all`
