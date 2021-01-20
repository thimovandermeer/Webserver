//
// Created by Thimo Van der meer on 20/01/2021.
//

#include "Response.h"
#include "Request.h"
#include "Requestconfig.h

Response::Response()
{
	_response = "";
	_content ="";
	_path = "";
	_code = 0;
}

void Response::checkMethod(Request &request, Requestconfig &requestconfig)
{
	_path = requestconfig.getpath();
	_code = 200;

	if(request.getMethod() == "GET")
		getMethod();
	if(request.getMethod() == "HEAD")
		headMethod();
	if(request.getMethod() == "POST")
		postMethod();
	if(request.getMethod() == "PUT")
		putMethod();
	if(request.getMethod() == "DELETE")
		deleteMethod();
	if (request.getMethod() == "CONNECT")
		connectMethod();
	if (request.getMethod() == "OPTIONS")
		optionsMethod();
	if (request.getMethod() == "TRACE")
		traceMethod();
}

void Response::getMethod()
{

}

Response::~Response()
{

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

