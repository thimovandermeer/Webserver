//
// Created by Thimo Van der meer on 20/01/2021.
//

#include "Response.h"
#include "request.hpp"
#include <fstream>
#include <unistd.h>



Response::Response()
{
	_response = "";
	_content ="";
	_path = "";
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
		   _code == rhs._code;
}

bool Response::operator!=(const Response &rhs) const
{
	return !(rhs == *this);
}

void Response::checkMethod(Request &request, RequestConfig &requestconfig)
{
	_path = requestconfig.getpath();
	_code = 200;
	if(request.getMethod() == 0)
		getMethod();
	if(request.getMethod() == 1)
		headMethod();
	if(request.getMethod() == 2)
		postMethod();
	if(request.getMethod() == 3)
		putMethod();
	if(request.getMethod() == 4)
		deleteMethod();
	if (request.getMethod() == 5)
		connectMethod();
	if (request.getMethod() == 6)
		optionsMethod();
	if (request.getMethod() == 7)
		traceMethod();
}

void Response::getMethod()
{
	std::ifstream file;
	// here the response header should me initiated

	// check if path to file exists
	const char *c = _path.c_str();
	if(access(c, F_OK) != 0)
		_code = 404;
	// check if you can open the file
	file.open(_path, std::ifstream::in);
	if(!file.is_open())
		_code = 403;
	// check if someone has permission to open the file // dont know if this is necessary tho

	// read file
	// safe stuff in file to content
	_content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
}

void Response::headMethod()
{
}

void Response::postMethod()
{
}

void Response::putMethod()
{
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

void Response::traceMethod()
{

}

std::ostream &operator<<(std::ostream &os, const Response &response)
{
	os << "_response: " << response._response << " _content: " << response._content << " _path: " << response._path
	   << " _code: " << response._code;
	return os;
}

