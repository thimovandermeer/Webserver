//
// Created by Thimo Van der meer on 09/02/2021.
//

#include <AppleEXR.h>
#include "CGI.hpp"

// default stuff
CGI::CGI(Request &request, server &server)
{
	this->_initEnvironment(request, server);
}

CGI::CGI(CGI &src)
{
	if (this != &src)
		this->_environment = src._environment;
	return ;
}

CGI::~CGI()
{
	;
}

CGI::CGI()
{
	// ERROR;
}

// public stuff

std::string CGI::executeCgi(std::string script) const
{
	// convert to char** for use in execve

	char **env = this->_convertEnv(); // try catch blok van maken in de toekomst

	int stdinSafe = dup(STDIN_FILENO);
	int stdoutSafe = dup(STDOUT_FILENO);

	int fds[2];
	pipe(fds);
	pid_t id = fork();
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	if (!id) {
		execve(script.c_str(), NULL, env);
		close(fds[0]);
		close(fds[1]);
		return (NULL);
	} else {
		int status;
		// here we wait for the child proces to finish
		waitpid(id, &status, 0);
		int ret = 1;
		char buffer[500];
		std::string outputStream;
		while (ret > 0) {
			ret = read(fds[0], buffer, 500);
			for (int i = 0; i < ret; i++) {
				outputStream += buffer[i];
			}
		}
		dup2(stdinSafe, STDIN_FILENO);
		dup2(stdoutSafe, STDOUT_FILENO);
		close(fds[0]);
		close(fds[1]);
		std::cout << outputStream << std::endl;
		return NULL;
	}
}

void CGI::_initEnvironment(Request &request, server &server)
{
	std::map<headerType, std::string> reqHeaders = request.getHeaders();
	if (reqHeaders.find(AUTHORIZATION) != reqHeaders.end())
		this->_environment["AUTH_TYPE"] = reqHeaders[AUTHORIZATION];
	this->_environment["CONTENT_LENGTH"] = request.getBody().length();
	if (request.getBody().empty())
		this->_environment["CONTENT_TYPE"] = "";
	else
		this->_environment["CONTENT_TYPE"] = request.getMethod();
	this->_environment["GATEWAY_INTERFACE"] = "EPIC CGI"; // search app
	this->_environment["PATH_INFO"] = request.getUri(); // SEARCH APP
	this->_environment["PATH_TRANSLATED"] = request.getUri(); // SEARCH APP
	this->_environment["QUERY_STRING"] = request.getCgiEnv(); // SEARCH APP
	this->_environment["REMOTE_ADDR"] = server.getHost(); // SEARCH APP
	this->_environment["REMOTE_IDENT"] = "Hello this is bullshit so we did not implement this";
	this->_environment["REMOTE_USER"] = "The same as above counts for this"; // SEARCH APP
	this->_environment["REQUEST_METHOD"] = request.getMethod(); // SEARCH APP
	this->_environment["REQUEST_URI"] = request.getUri(); // search app
	this->_environment["SCRIPT_NAME"] = request.getUri(); // search app
	if (reqHeaders.find(HOST) != reqHeaders.end())
		this->_environment["SERVER_NAME"] = reqHeaders[HOST];
	else
		this->_environment["SERVER_NAME"] = this->_environment["REMOTE_ADDR"];
	this->_environment["SERVER_PORT"] = server.getPortNr(); // search app
	this->_environment["SERVER_PROTOCOL"] = "HTTP/1.1"; // search app
	this->_environment["SERVER_SOFTWARE"] = "Merel Jonas Thimo Epic webserver huts"; // search app
}




char **CGI::_convertEnv() const
{
	char **env = new char*[this->_environment.size() + 1];
	int j = 0;
	if (!env)
	{
		delete[] env;
		return (NULL);
	}
	 std::map<std::string, std::string>::const_iterator i = this->_environment.begin();
	while(i != this->_environment.end())
	{
		std::string temp = i->first + "=" + i->second;
		env[j] = strdup(temp.c_str());
		i++;
		j++;
	}
	env[j] = NULL;
	return (env);
}

