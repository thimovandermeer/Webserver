#ifndef WEBSERV_HPP
# define WEBSERV_HPP
# include <iostream>
# include <iostream>
# include <cstdlib>
# include <cstring>
# include <sys/utsname.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <cstdio>
# include <fstream>
# include <sstream>

extern std::string	g_name;
extern bool			g_leaks;

void	leaksExit(const std::string &errMsg, int code);

void	openConfig(int ac, char **av);

#endif
