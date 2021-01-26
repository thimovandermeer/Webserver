//
// Created by Thimo Van der meer on 20/01/2021.
//

#include "../srcs/Response.hpp"
#include "../srcs/request.hpp"
#include "../srcs/ResponseHeader.hpp"
#include <fstream>
#include <unistd.h>



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

bool Response::operator==(const Response &rhs) const
{
	return _response == rhs._response &&
		   _content == rhs._content &&
		   _path == rhs._path &&
		   _code == rhs._code &&
			_contentType == rhs._contentType;
}

bool Response::operator!=(const Response &rhs) const
{
	return !(rhs == *this);
}

Response &Response::operator=(const Response &src)
{
	_response = src._response;
	_content = src._content;
	_path = src._path;
	_code = src._code;
	return (*this);
}

void Response::checkMethod(Request &request, RequestConfig &requestconfig)
{
	_path = requestconfig.getpath();
	_code = 200;
	_contentType = request.getContentType();
	if(request.getMethod() == 0)
		getMethod(); // done
	if(request.getMethod() == 1)
		headMethod(); //
	if(request.getMethod() == 2)
		postMethod();
	if(request.getMethod() == 3)
		putMethod(request.getBody());
	if(request.getMethod() == 4)
		deleteMethod();
	if (request.getMethod() == 5)
		connectMethod();
	if (request.getMethod() == 6)
		optionsMethod();
	if (request.getMethod() == 7)
		traceMethod(request);
}

void 	Response::readContent()
{
	std::ifstream file;

	const char *c = _path.c_str();
	if(access(c, F_OK) != 0)
		_code = 404;
	file.open(_path, std::ifstream::in);
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
	if(access(c, F_OK) != 0)
		_code = 404;
	file.open(_path, std::ofstream::out | std::ofstream::trunc);
	if(!file.is_open())
		_code = 403;
	file << content;
	file.close();

}
void Response::getMethod()
{
	ResponseHeader header;
	// here the response header should me initiated
	// so something like create response header
	readContent();
	_response = header.getHeader(_content, _path, _code, _contentType) + _content;

}

void Response::headMethod()
{
	ResponseHeader header;
	readContent();
  	_response = header.getHeader(_content, _path, _code, _contentType);
}

void Response::postMethod()
{
	// need more knowledge about CGI
}

void Response::putMethod(std::string content)
{
	(void)content;
}

void Response::deleteMethod()
{

}

void Response::connectMethod()
{
}

void Response::optionsMethod()
{

}

void Response::traceMethod(Request &request)
{
	(void)request;
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

