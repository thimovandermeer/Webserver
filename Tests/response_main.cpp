/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_main.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thvan-de <thvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/25 09:53:17 by thvan-de      #+#    #+#                 */
/*   Updated: 2021/01/25 10:50:09 by thvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../srcs/request.hpp"
#include "../srcs/Response.hpp"
#include "../srcs/utils.hpp"


#include <stdio.h>
// in here we put all our tests



void testHeadMethod()
{

}

void testPostMethod()
{

}

void testDeleteMethod()
{

}

void testConnectMethod()
{

}

void testOptionsMethod()
{

}

void testTraceMethod()
{

}

void testHeaders(Response *response)
{
	std::cout << response->getResponse() << std::endl;
}

void	testGetMethod()
{
	Request request("GET path.txt HTTP/1.1\r\n"
					"Host: eloquentjavascript.net\r\n"
					"Date:leukeserverdit\r\n"
					"Allow: jajaja \r\n"
					"Retry_After: waaromdoejijhetniet\r\n"
				  	"Content_type: .json\r\n"
					"User_Agent: The Imaginary Browser\r\n\r\n"

					"EPICSICKE SHIUT OUWW\r\n"
					"hoi\r\n"
	);
	request.parseRequest();
	Response 		response;
	RequestConfig	requestConfig;
	std::string path;
	path = "path.txt";
	requestConfig.setpath(path);
	response.checkMethod(request, requestConfig);
	// hier komen functies die goed gaan dus hier moeten specifieke teksten gereturnt worden
	std::cout << response.getContent() << std::endl;
	std::cout << response.getCode() << std::endl;
	testHeaders(&response);
}

int main()
{



//	request.setMethod("GET");
	// testing the headers
	// testing all method

	// test get method
	testGetMethod();
	testHeadMethod();
	testPostMethod();
	testDeleteMethod();
	testConnectMethod();
	testOptionsMethod();
	testTraceMethod();
}
