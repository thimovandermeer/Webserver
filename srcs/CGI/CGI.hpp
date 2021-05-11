#ifndef CGI_HPP
#define CGI_HPP

#include "../Request/request.hpp"
#include "../Server/server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/wait.h>
#include <sstream>
#include <iostream>

class CGI
{
public:
	CGI(std::string &path, Request &request, server &server);
	CGI(CGI &src);
	~CGI();
	CGI();
	void executeGCI(std::string &body);
	void		setupIn();
	std::string readOutput();

private:
	void 								_initEnvironment(Request &request, server &server);
	void 								_convertEnv();
	std::string							_setRedirectStatus();
	int		_fileIn;
	int		_fileOut;
	int		_fileRet;
private:

	std::map<std::string, std::string> 	_environment;
	char								**_env;
	std::string 						_path;
	pid_t 								_pid;
	filetype							_type;
};

#endif
