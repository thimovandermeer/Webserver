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
           Utils/Base64.cpp \
           Utils/utils.cpp \
           webserv.cpp
SRCS = $(addprefix srcs/, $(_SRCS))
INCLUDES = $(addprefix srcs/, $(_INCLUDE))
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CXXFLAGS) -I. -L. $(SRCS) -o $(NAME)
    # @$(CC) $(CXXFLAGS) -I. -L. $(SRCS) -o $(NAME)
#        @$(CC) $(CFLAGS) -I. $(SRCS) -o $(NAME)
#     @$(CXX) $(CXXFLAGS) -I. $(SRCS) -o $(NAME)

#
# clean:
#     @rm -f ../html_pages/Downloads/Download_1
clean:
	@$(RM) $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

# # **************************************************************************** #
# #                                                                              #
# #                                                         ::::::::             #
# #    Makefile                                           :+:    :+:             #
# #                                                      +:+                     #
# #    By: thvan-de <thvan-de@student.codam.nl>         +#+                      #
# #                                                    +#+                       #
# #    Created: 2020/07/25 17:28:30 by pde-bakk      #+#    #+#                  #
# #    Updated: 2021/01/25 10:59:18 by thvan-de      ########   odam.nl          #
# #                                                                              #
# # **************************************************************************** #

# NAME = webserv
# CC = clang++
# CXXFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c++98
# INCLUDES = CGI/CGI.hpp \
# 		   Cluster/serverCluster.hpp \
# 		   Request/request.hpp \
# 		   Response/getPath.hpp \
# 		   Response/Response.hpp \
# 		   Response/responseHeader.hpp \
# 		   Server/connection.hpp \
# 		   Server/location.hpp \
# 		   Server/server.hpp \
# 		   Utils/Base64.hpp \
# 		   Utils/defines.hpp \
# 		   Utils/utils.hpp \
# 		   webserv.hpp

# SRCS =	   CGI/CGI.cpp \
# 		   Cluster/serverCluster.cpp \
# 		   Request/request.cpp \
# 		   Response/getPath.cpp \
# 		   Response/Response.cpp \
# 		   Response/responseHeader.cpp \
# 		   Server/connection.cpp \
# 		   Server/location.cpp \
# 		   Server/serverBasic.cpp \
# 		   Server/serverUtility.cpp \
# 		   Utils/Base64.cpp \
# 		   Utils/utils.cpp \
# 		   webserv.cpp
# # SRCS = $(addprefix srcs/, $(_SRCS))
# # INCLUDES = $(addprefix srcs/, $(_INCLUDE))
# OBJS = $(SRCS:.cpp=.o)

# all: $(NAME)

# $(NAME): $(OBJS)
# #        @$(CC) $(CFLAGS) -I. $(SRCS) -o $(NAME)
# #     @$(CXX) $(CXXFLAGS) -I. $(SRCS) -o $(NAME)

# #
# # clean:
# #     @rm -f ../html_pages/Downloads/Download_1

# fclean: clean
# 	@rm -rf $(NAME)

# re: fclean all
