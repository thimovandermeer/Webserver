//
// Created by Thimo Van der meer on 09/02/2021.
//

#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "../Request/request.hpp"
#include "../Server/server.hpp"

class CGI
{
public:
	class CgiError : public std::runtime_error {
	public:
		explicit CgiError(const char* w);
	};
	class PipeSetupFailed : public CgiError {
	public:
		PipeSetupFailed();
	};
	class ForkFailed : public CgiError {
	public:
		ForkFailed();
	};
	// zometeen aanpassen
	int 								_outputRedirFds[2];
	int 								_inputRedirFds[2];

	CGI(std::string &path, Request &request, server &server);
	CGI(CGI &src);
	~CGI();
	CGI();
	void								executeGCI();
private:
	void 								_initEnvironment(Request &request, server &server);
	void 								_convertEnv();
	void 								_createPipe();
	bool 								_forkProcess();
	void 								_setupRedir();
	void 								_switchProcess();

private:

	std::map<std::string, std::string> 	_environment;
	char								**_env;
	std::string 						_path;

	pid_t 								_pid;
};

#endif //WEBSERV_CGI_HPP
