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

#include "../request.hpp"
#include "../Response.h"

#include <stdio.h>
// in here we put all our tests

void	test_all_methods()
{

}
int main()
{
	Response Response;
	RequestConfig requestConfig;
	Request	request;
	std::string path = "hallo";
	requestConfig.setpath(path);
	Response.checkMethod(request, requestConfig);


}