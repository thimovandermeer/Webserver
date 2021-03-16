#ifndef CGI_HPP
#define CGI_HPP

#include "../Request/request.hpp"
#include "../Server/server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <fstream>
#include <sys/wait.h>
#include <sstream>

class CGI
{
public:
	CGI(std::string &path, Request &request, server &server);
	CGI(CGI &src);
	~CGI();
	CGI();
	std::string executeGCI(std::string &body);

private:
	void 								_initEnvironment(Request &request, server &server);
	void 								_convertEnv();

private:

	std::map<std::string, std::string> 	_environment;
	char								**_env;
	std::string 						_path;
	pid_t 								_pid;
};

#endif
