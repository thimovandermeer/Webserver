#include <map>
#include "server.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstring>
#include "../Utils/utils.hpp"
#include "../Request/request.hpp"
#include "../Response/Response.hpp"

void	server::startListening()
{
	this->_socketFd = socket(PF_INET, SOCK_STREAM, 0);
	if (this->_socketFd < 0)
	{
		std::cerr << "socket error" << std::endl;
		throw server::syscallErrorException();
	}
	bzero(&this->_addr, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(this->_portNr);
	// do something with host
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY); // this can be the IP address

//	 clear port if it is in use
	int	ret;
	int options = 1;
	ret = setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options));
	if (ret < 0)
	{
		std::cerr << "setsockopt error" << std::endl;
		throw server::syscallErrorException();
	}

	ret = bind(this->_socketFd, (sockaddr*)&(this->_addr), sizeof(this->_addr));
	if (ret < 0)
	{
		std::cerr << "bind error: possibly you have multiple servers listening on the same socket" << std::endl;
		throw server::syscallErrorException();
	}
	ret = listen(this->_socketFd, NR_OF_CONNECTIONS);
	if (ret < 0)
	{
		std::cerr << "listen error" << std::endl;
		throw server::syscallErrorException();
	}
}

void 	server::acpt()
{
	struct sockaddr connectingAddr;
	socklen_t		addressLen;
	int				i;

	for (i = 0; i < NR_OF_CONNECTIONS; i++)
	{
		if (this->connections[i].acceptFd == -1)
			break;
	}
	if (i == NR_OF_CONNECTIONS)
	{ ; } // too many connections, should never happen
	this->connections[i].acceptFd = accept(this->_socketFd, &connectingAddr, &addressLen);
	if (this->connections[i].acceptFd == -1)
		std::cerr << "Could not create fd" << std::endl;
	this->connections[i].timeLastRead = getTime();
//	this->_acceptFds.push_back((accept(this->_socketFd, &connectingAddr, &addressLen)));
//	if (*this->_acceptFds.rbegin() == -1)
//		std::cerr << "Could not create fd" << std::ençdl; // dit zometeen aanpassen naar try catch
	//	fcntl(this->_acceptFd, F_SETFL, O_NONBLOCK);
}

int hasBody(std::string request)
{
	if (request.find("Transfer-Encoding: chunked") != std::string::npos)
		return 1;
	else if (request.find("Content-Length") != std::string::npos)
		return 2;
	return 0;
}

void	ft_bzero(char *buf, size_t n)
{
	size_t	i;
	char	*ptr;

	ptr = buf;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

void server::generateResponse(int index)
{
	std::cout << "==REQUEST==" << std::endl;
//	std::cout << connections[index].acceptBuffer << std::endl;
	int len1 = connections[index].acceptBuffer.length();
	int len2 = len1 > 500 ? 500 : len1;
	write(1, connections[index].acceptBuffer.c_str(), len2);
	std::cout << "==end==" << std::endl;
	Request	request(connections[index].acceptBuffer);
	Response resp(request, *this);
	resp.setupResponse(request, *this);
	_response = resp.getResponse();
}
