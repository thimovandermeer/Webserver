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
#include <fstream>
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
	std::ifstream example("../test_files/request/get/GET_example");
	std::string string((std::istreambuf_iterator<char>(example)),
					std::istreambuf_iterator<char>());
	Request request(string);
	request.parseRequest();
	Response 		response;
	Server 			server;
	std::string root = "";
	server.setRoot(root);
	response.checkMethod(request, server);
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
