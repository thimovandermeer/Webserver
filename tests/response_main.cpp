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
//#include "Catch2.h"

#include <stdio.h>
// in here we put all our tests

void	testGetMethod()
{
	// test time function;
	std::cout << getTime() << std::endl;
//	Response 		Response;
//	RequestConfig	requestConfig;
//	Request			request;
//	std::string path = "path.txt";
//	requestConfig.setpath(path);
//	request.setMethod("GET");
//	Response.checkMethod(request, requestConfig);
//
//	// hier komen functies die goed gaan dus hier moeten specifieke teksten gereturnt worden
//	std::cout << Response.getContent() << std::endl;
//	std::cout << Response.getCode() << std::endl;
//	// hier komen functies die fout gaan dus hier moeten specifieke exit codes komen
//	path = "doei.txt";
//	requestConfig.setpath(path);
//	Response.checkMethod(request,requestConfig);
//	std::cout << Response.getContent() << std::endl;
//	std::cout << Response.getCode() << std::endl;
}

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

int main()
{

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
