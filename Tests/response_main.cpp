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

void	setRequests()
{
	Request *request;

	for (int i = 0; i < 10; i++)
	{
		request[i] = new request;
	}
}

void	testGetMethod()
{
	Response 		response;
	RequestConfig	requestConfig;
	Request			request;
	std::string path = "path.txt";
	requestConfig.setpath(path);
	request.setMethod("GET");

	response.checkMethod(request, requestConfig);

	// hier komen functies die goed gaan dus hier moeten specifieke teksten gereturnt worden
	std::cout << response.getContent() << std::endl;
	std::cout << response.getCode() << std::endl;
	testHeaders(&response);
	// hier komen functies die fout gaan dus hier moeten specifieke exit codes komen
	path = "doei.txt";
	requestConfig.setpath(path);
	response.checkMethod(request, requestConfig);
	std::cout << response.getContent();
	std::cout << response.getCode();
	testHeaders(&response);
}

//int main()
//{
//	std::string request = "Hallo hfkjhlkhgw";
//
//	request request;
//	request.parse(request);
//	// testing the headers
//	// testing all method
//
//	// test get method
//	testGetMethod();
//	testHeadMethod();
//	testPostMethod();
//	testDeleteMethod();
//	testConnectMethod();
//	testOptionsMethod();
//	testTraceMethod();
//}
