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

NAME = webserv.out

ifdef COMPILER
 CXX = $(COMPILER)
endif
INCLUDE = $(shell find srcs -type d | sed s/^/-I/)
CXXFLAGS = -W -Wall -Wextra -Werror -pedantic -ansi -std=c++98
ifdef DEBUG
 CXXFLAGS += -g -fsanitize=address #-fsanitize=undefined -fsanitize=alignment -fsanitize=bounds -fsanitize=null -fsanitize=return
else
 CXXFLAGS += -Ofast
endif

TESTDIR = tests
FILES = main.cpp

response request: fclean
	$(CXX) $(CXXFLAGS) $(TESTDIR)/$@_$(FILES) -o $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME) $(NAME).dSYM

fuckingclean: fclean
	@rm -f $(TESTDIR)/std.txt $(TESTDIR)/webserv.txt $(TESTDIR)/diff.txt \
	$(TESTDIR)/*stdmain.cpp

re: fclean
