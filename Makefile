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
INCLUDE = Response.hpp request.hpp
CXXFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c++98


TESTDIR = tests
MAINFILES = main.cpp
FILES = Response.cpp  request.cpp
response request: fclean
	$(CXX) $(CXXFLAGS) $(TESTDIR)/$@_$(MAINFILES) $(FILES) -o $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME) $(NAME).dSYM

fuckingclean: fclean
	@rm -f $(TESTDIR)/std.txt $(TESTDIR)/webserv.txt $(TESTDIR)/diff.txt \
	$(TESTDIR)/*stdmain.cpp

re: fclean
