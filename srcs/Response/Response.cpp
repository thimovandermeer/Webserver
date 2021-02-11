//
// Created by Thimo Van der meer on 20/01/2021.
//

#include "Response.hpp"
#include "../Request/request.hpp"
#include "ResponseHeader.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include "../Utils/utils.hpp"


Response::Response() : _status(0)
{

}

Response::~Response()
{

}

Response &Response::operator=(const Response &src)
{
	_response = src._response;
	_content = src._content;
	_path = src._path;
	_status = src._status;
	return (*this);
}

std::string Response::getPath(server &server, Request &request)
{
	// request kant
		// vraagt om specifieke location
	// server kant
		// heeft location geconfigureerd
	// dit moet gematcht worden

	std::string root = server.getRoot();

	// from the request side i need the path
	std::string path =	request.getUri();
	std::string locMatch = request.getHost();
//	location *loc = server.findLocation(path);
	std::string ret;
//	if (!loc)
//		ret = root + path.substr(locMatch.length());
//	else
//	{
	ret = root + path;
//	}
//	std::string temp;
	if ((*ret.end() - 1) == '/')
		ret.erase(ret.end() - 1);
//	temp = removeAdjacentSlashes(ret);
	return ret;
}

void Response::checkMethod(Request &request, server &server)
{
	_path = getPath(server, request);
	_status = request.getStatus();
	if (this->_status != 200)
	{
//		createErrorPage();
		return;

	}
	_contentType = request.getContentType();
	if(request.getMethod() == 0)
		getMethod(); // done
	if(request.getMethod() == 1)
		headMethod(); // done
	if(request.getMethod() == 2)
		postMethod(request.getBody());
	if(request.getMethod() == 3)
		putMethod(request.getBody()); // done
}

void 	Response::readContent()
{
	std::ifstream file;

	const char *c = _path.c_str();
	if(access(c, F_OK) != 0 && _status == 200)
		_status = 404;
	file.open(_path, std::ifstream::in);
	if(!file.is_open() && _status == 200)
		_status = 403;
	_content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
}

void 	Response::writeContent(std::string content)
{
	std::ofstream file;
	if (_status == 200)
    	_status = 204;
	const char *c = _path.c_str();
	if(access(c, F_OK) == 0 && _status == 200)
		_status = 201;
	file.open(_path, std::ofstream::out | std::ofstream::trunc);
	if(!file.is_open() && _status == 200)
		_status = 403;
	file << content;
	file.close();

}
void Response::getMethod()
{
	readContent();
	ResponseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status) + _content;

}

void Response::headMethod()
{
	readContent();
	ResponseHeader header(_content, _path, _status, _contentType);
  	_response = header.getHeader(_status);
}

void Response::postMethod(std::string content)
{
	std::ofstream file;
	if (_status == 200)
    	_status = 204;
	const char *c = _path.c_str();
	if(access(c, F_OK) == 0 && _status == 200)
		_status = 201;
	file.open(_path, std::ios::out | std::ios::app);
	if(!file.is_open() && _status == 200)
		_status = 403;
	file << content;
	file.close();
	ResponseHeader header(content, _path, _status, _contentType);
	_response = header.getHeader(_status); // here we got a potential bug
	// need more knowledge about CGI
}

void Response::putMethod(std::string content)
{
	writeContent(content);
	ResponseHeader header(content, _path, _status, _contentType);
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

int 				Response::getCode()
{
	return _status;
}

std::ostream &operator<<(std::ostream &os, const Response &response)
{
	os << "_response: " << response._response << " _content: " << response._content << " _path: " << response._path
	   << " _status: " << response._status;
	return os;
}

