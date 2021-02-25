//
// Created by Thimo Van der meer on 20/01/2021.
//

#include "Response.hpp"
#include "responseHeader.hpp"
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <sys/stat.h>

Response::Response(Request &req, server &serv) :
	_path(getPath(serv, req, *this)), // delete hardcoded
	_contentType(req.getContentType()),
	_CGI(_path, req, serv),
	_useCGI(req.getCgi()),
	_status(req.getStatus()),
	_method(req.getMethod())
{
    _errorMessage[204] = "No Content";
    _errorMessage[400] = "Bad Request";
    _errorMessage[403] = "Forbidden";
    _errorMessage[404] = "Not Found";
    _errorMessage[405] = "Method Not Allowed";
}

Response::Response(const Response &src)
{
	*this = src;
}

Response::~Response()
{

}

Response &Response::operator=(const Response &src)
{
	_response = src._response;
	_content = src._content;
	_path = src._path;
	_contentType = src._contentType;
	_CGI = src._CGI;
	_useCGI = src._useCGI;
	_status = src._status;
	_errorMessage = src._errorMessage;
	_method = src._method;
	return (*this);
}

bool	Response::isMethodAllowed()
{
	if (!this->currentLoc)
		return (false);
	std::vector<std::string>::iterator it;
	std::vector<std::string> vc = this->currentLoc->getMethods();
	for (it = vc.begin(); it < vc.end(); it++)
	{
		if ((*it) == this->_method)
			return (true);
	}
	this->_status = 405;
	return (false);
}

void	Response::setupResponse(Request &request, server &serv) {
	this->setStatus(request.getStatus());
	if(_method == "GET")
	{
		if (this->isMethodAllowed())
			getMethod(); // done
	}
	if(_method == "HEAD")
	{
		if (this->isMethodAllowed())
			headMethod(); // done
	}
	if(_method == "POST")
	{
		if (this->isMethodAllowed())
			postMethod(request.getBody());
	}
	if(_method == "PUT")
	{
		if (this->isMethodAllowed())
			putMethod(request.getBody()); // done
	}
	if (this->_status >= 299)
	{
		this->errorPage(serv);
	}
}

void 	Response::readContent()
{
	if (_useCGI == true)
	{
		_content = _CGI.executeGCI();
		return ;
	}
	std::ifstream file;
	struct stat statBuf;

	if(stat(_path.c_str(), &statBuf) != 0)
		return (this->setStatus(404));
	file.open(this->_path, std::ifstream::in);
	if(!file.is_open())
		return (this->setStatus(403));
	if(stat(_path.c_str(), &statBuf) != 0 && _status == 200)
		return (this->setStatus(404));
	file.open(_path, std::ifstream::in);
	if(!file.is_open() && _status == 200)
		return (this->setStatus(403));
	if (this->_status == 200)
	_content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
}

void 	Response::writeContent(std::string content)
{
	std::ofstream file;
//	if (_status == 200)
//    	_status = 204;
	struct stat statBuf;
	if(stat(_path.c_str(), &statBuf) == 0 && _status == 200)
		this->setStatus(201);
	file.open(_path, std::ofstream::out | std::ofstream::trunc);
	if(!file.is_open() && _status == 200)
		return (this->setStatus(403));
	file << content;
	file.close();

}

void    Response::createErrorPage(std::string *pageData)
{
    size_t found = 1;
	while (found != std::string::npos)
	{
		found = pageData->find("ERROR_CODE");
		if (found == std::string::npos)
			break;
		std::stringstream stat;
		stat << this->_status;
		std::string statstr;
		stat >> statstr;
		pageData->replace(found, 10, statstr);
	}
	found = 1;
    while (found != std::string::npos)
    {
        found = pageData->find("MESSAGE");
        if (found == std::string::npos)
            break;
        std::stringstream stat;
        std::map<int, std::string>::iterator it = _errorMessage.find(_status);
        if (it == this->_errorMessage.end())
        	pageData->replace(found, 7, "unknown error");
        else
        	pageData->replace(found, 7, it->second);
    }
}

void	Response::errorPage(server &serv)
{
	int	        fd;
	int         ret = 4096;
	char        buff[4096];
	std::string	pageData;
	std::string	pathToPage;

	pathToPage = serv.getRoot() + serv.getErrorPage();
	fd = open(pathToPage.c_str(), O_RDONLY);
	if (fd < 0)
		pageData = "Problem serving error. Perhaps the error page was setup incorrectly.";
	else
	{
		while (ret == 4096)
		{
			ret = read(fd, buff, 4095);
			buff[ret] = 0;
			pageData += buff;
		}
		close(fd);
		createErrorPage(&pageData);
	}
	this->_content.clear();
	this->_content = pageData;
	responseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status) + _content;
}

void Response::getMethod()
{
	readContent();
	responseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status) + _content;

}

void Response::headMethod()
{
	readContent();
	responseHeader header(_content, _path, _status, _contentType);
  	_response = header.getHeader(_status);
}

void Response::postMethod(std::string content)
{
	if(_useCGI == true) {
		readContent();
		responseHeader header(content, _path, _status, _contentType);
		_response = header.getHeader(_status) + _content;
		return;
	}
	std::ofstream file;
	file.open(_path, std::ios::out | std::ios::app);
	if(!file.is_open() && _status == 200)
		this->setStatus(403);
//	this->setStatus(204);
	struct stat statBuf;
	if(stat(_path.c_str(), &statBuf) == 0 && _status == 200)
		this->setStatus(201);
	file << content;
	file.close();
	responseHeader header(content, _path, _status, _contentType);
	_response = header.getHeader(_status); // here we got a potential bug
	// need more knowledge about CGI
}

void Response::putMethod(std::string content)
{
	writeContent(content);
	responseHeader header(content, _path, _status, _contentType);
	_response = header.getHeader(_status); // here we got a potential bug
}

// getters
std::string 		Response::getContent()
{
	return _content;
}

const std::string 	&Response::getResponse() const
{
	return _response;
}

int 				Response::getStatus() const
{
	return _status;
}

void				Response::setStatus(int status)
{
	if (this->_status >= 400)
		return;
	this->_status = status;
}

std::ostream &operator<<(std::ostream &os, const Response &response)
{
	os << "_response: " << response._response << " _content: " << response._content << " _path: " << response._path
	   << " _status: " << response._status;
	return os;
}

