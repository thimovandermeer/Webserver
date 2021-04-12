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
           Utils/Base64.cpp \
           Utils/utils.cpp \
           main.cpp
SRCS = $(addprefix srcs/, $(_SRCS))
#_OBJS = $(_SRCS:CGI/%=%), $(_SRCS:Cluster/%=%), $(_SRCS:Request/%=%), $(_SRCS:Response/%=%), $(_SRCS:Server/%=%), $(_SRCS:Utils/%=%), $(_SRCS:.cpp=.o)
#_OBJS = $($($($($($($(_SRCS:.cpp=.o):Utils/%=%):Server/%=%):Response/%=%):Request/%=%):Cluster/%=%):CGI/%=%)
_OBJS0 = $(_SRCS:CGI/%=%)
_OBJS1 = $(_OBJS0:Cluster/%=%)
_OBJS2 = $(_OBJS1:Request/%=%)
_OBJS3 = $(_OBJS2:Response/%=%)
_OBJS4 = $(_OBJS3:Server/%=%)
_OBJS5 = $(_OBJS4:Utils/%=%)
_OBJS6 = $(_OBJS5:.cpp=.o)

OBJS = $(addprefix objects/, $(_OBJS6))

all: $(NAME)

objects/%.o: srcs/*/%.cpp
	@mkdir -p objects
	@$(CC) $(CXXFLAGS) -c $< -o $@

objects/%.o: srcs/%.cpp
	@mkdir -p objects
	@$(CC) $(CXXFLAGS) -c $< -o $@

vars:
	@echo $(OBJS)

$(NAME): $(OBJS)
	@$(CC) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@rm -rf $(NAME)
	@rm -f html_pages/Downloads/multiple_same
	@rm -f html_pages/Downloads/file_should_exist_after
	@rm -f html_pages/Downloads/Download_1
	@rm -f html_pages/Downloads/Download_2

re: fclean all
