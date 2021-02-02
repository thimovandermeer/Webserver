NAME = catch_test
SRCS = catch_main.cpp test_response.cpp ../srcs/request.cpp ../srcs/Response.cpp ../srcs/ResponseHeader.cpp
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -std=c++11

all: $(NAME)

$(NAME): $(OBJS)
	@echo $(ECHO) "$(PREFIX)$(GREEN) Bundling executable... $(END)$(NAME)"
	@$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDE) -o $@

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

run: all
	./$(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
