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
	CGI(Request &request, server &server);
	CGI(CGI &src);
	~CGI();

	std::string	executeCgi(std::string script) const;
private:
	CGI();
	void 								_initEnvironment(Request &request, server &server);
	char 								**_convertEnv() const;

private:
	std::map<std::string, std::string> 	_environment;
	std::string 						_body;
};

#endif //WEBSERV_CGI_HPP
