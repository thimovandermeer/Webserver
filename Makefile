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
_SRCS =    CGI/CGI.cpp \
           Cluster/serverCluster.cpp \
           Request/request.cpp \
           Response/getPath.cpp \
           Response/Response.cpp \
           Response/responseHeader.cpp \
           Server/connection.cpp \
           Server/location.cpp \
           Server/parser.cpp \
           Server/serverBasic.cpp \
           Server/serverUtility.cpp \
		   Server/parser.cpp \
           Utils/Base64.cpp \
           Utils/utils.cpp \
           webserv.cpp
SRCS = $(addprefix srcs/, $(_SRCS))
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CXXFLAGS) -I. $(SRCS) -o $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@rm -rf $(NAME)
	@rm -f html_pages/Downloads/multiple_same
	@rm -f html_pages/Downloads/file_should_exist_after
	@rm -f html_pages/Downloads/Download_1
	@rm -f html_pages/Downloads/Download_2

re: fclean all
