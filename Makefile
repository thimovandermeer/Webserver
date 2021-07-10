# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: thvan-de <thvan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/07/25 17:28:30 by pde-bakk      #+#    #+#                  #
#    Updated: 2021/07/10 12:23:38 by thvan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = g++
CXXFLAGS = -W  -pedantic -std=c++03
OBJDIR = objects

_SRCS =	CGI/CGI.cpp \
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

_OBJS = $(addprefix $(OBJDIR)/, $(notdir $(_SRCS)))
OBJS = $(_OBJS:.cpp=.o)

all: $(NAME)

objects/%.o: srcs/*/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

objects/%.o: srcs/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJDIR)


fclean: clean
	@rm -rf $(NAME)
	@rm -f html_pages/Downloads/multiple_same
	@rm -f html_pages/Downloads/file_should_exist_after
	@rm -f html_pages/Downloads/Download_1
	@rm -f html_pages/Downloads/Download_2

re: fclean all
