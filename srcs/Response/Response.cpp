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


Response::Response()
{
	_response = "";
	_content ="";
	_path = "";
	_contentType = "";
	_code = 0;
}

Response::~Response()
{

}

Response &Response::operator=(const Response &src)
{
	_response = src._response;
	_content = src._content;
	_path = src._path;
	_code = src._code;
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
	std::string location = request.getHost();
	// the return value will be root + alias + path.substr(locationName.length)
	std::string ret;
//	if (!location.empty())
//		ret = root + path.substr(location.length());
//	else
	// functie jonas hier inzetten return location of leeg
	// check if not empty en daarna kijken of het een match is
	// geen match is goede exit code returnen
	ret = root + path;
	std::string temp;
	temp = removeAdjacentSlashes(ret);
	return temp;
}

void Response::checkMethod(Request &request, server &server)
{
	_path = getPath(server, request);
	_code = 200;
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
	if(access(c, F_OK) != 0)
		_code = 404;
	file.open(this->_path, std::ifstream::in);
	if(!file.is_open())
		_code = 403;
	_content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
}

void 	Response::writeContent(std::string content)
{
	std::ofstream file;
	_code = 204;
	const char *c = _path.c_str();
	if(access(c, F_OK) == 0)
		_code = 201;
	file.open(_path, std::ofstream::out | std::ofstream::trunc);
	if(!file.is_open())
		_code = 403;
	file << content;
	file.close();

}
void Response::getMethod()
{
	readContent();
	ResponseHeader header(_content, _path, _code, _contentType);
	_response = header.getHeader(_code) + _content;

}

void Response::headMethod()
{
	readContent();
	ResponseHeader header(_content, _path, _code, _contentType);
  	_response = header.getHeader(_code);
}

void Response::postMethod(std::string content)
{
	std::ofstream file;
	_code = 204;
	const char *c = _path.c_str();
	if(access(c, F_OK) == 0)
		_code = 201;
	file.open(_path, std::ios::out | std::ios::app);
	if(!file.is_open())
		_code = 403;
	file << content;
	file.close();
	ResponseHeader header(content, _path, _code, _contentType);
	_response = header.getHeader(_code); // here we got a potential bug
}

void Response::putMethod(std::string content)
{
	writeContent(content);
	ResponseHeader header(content, _path, _code, _contentType);
	_response = header.getHeader(_code); // here we got a potential bug
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
	return _code;
}

std::ostream &operator<<(std::ostream &os, const Response &response)
{
	os << "_response: " << response._response << " _content: " << response._content << " _path: " << response._path
	   << " _code: " << response._code;
	return os;
}

