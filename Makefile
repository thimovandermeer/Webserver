# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: thvan-de <thvan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/07/25 17:28:30 by pde-bakk      #+#    #+#                  #
#    Updated: 2021/04/07 15:24:42 by thvan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = clang++
CXXFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c++98
_INCLUDES = CGI/CGI.hpp \
           Cluster/serverCluster.hpp \
           Request/request.hpp \
           Response/getPath.hpp \
           Response/Response.hpp \
           Response/responseHeader.hpp \
           Server/connection.hpp \
           Server/location.hpp \
           Server/server.hpp \
		   Server/parser.hpp \
           Utils/Base64.hpp \
           Utils/defines.hpp \
           Utils/utils.hpp \
           webserv.hpp

_SRCS =    CGI/CGI.cpp \
           Cluster/serverCluster.cpp \
           Request/request.cpp \
           Response/getPath.cpp \
           Response/Response.cpp \
           Response/responseHeader.cpp \
           Server/connection.cpp \
           Server/location.cpp \
           Server/serverBasic.cpp \
           Server/serverUtility.cpp \
		   Server/parser.cpp \
           Utils/Base64.cpp \
           Utils/utils.cpp \
           webserv.cpp
SRCS = $(addprefix srcs/, $(_SRCS))
INCLUDES = $(addprefix srcs/, $(_INCLUDE))
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CXXFLAGS) -I. -L. $(SRCS) -o $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
