#include <map>
#include "server.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
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

bool	isEnd(std::string &request)
{
	int	len = request.length();
	bool	needsBody = false;

	if (request.find("Transfer-Encoding: chunked") != std::string::npos)
		needsBody = true;
	if (len < 4)
		return (false);

	if (needsBody)
	{
		size_t loc  = request.find("\r\n\r\n");
		if (request.find("\r\n\r\n", loc + 1) != std::string::npos)
			return (true);
		return (false);
	}

	if (request[len - 4] == '\r' && request[len - 3] == '\n' &&request[len - 2] == '\r' &&request[len - 1] == '\n')
		return (true);
	return (false);
}

#define BUFFSIZE 4095
std::string 		server::receive() const
{
	char		buffer[BUFFSIZE + 1];
	std::string request;
	int 		ret = BUFFSIZE;

	while(ret > 0)
	{
		std::cout << "reading..." << std::endl;
		ret = read(_acceptFd, buffer, BUFFSIZE);
		buffer[ret] = 0;
		if (ret == -1)
		{
			std::cerr << "recv error" << std::endl;
			throw server::syscallErrorException();
		}
		request += std::string(buffer);
		if (isEnd(request))
			break;
	}
	std::cout << "==REQUEST==" << std::endl;
	std::cout << request << std::endl;
	std::cout << "==end==" << std::endl;
	return (request);
}

void 		server::sendData(const std::string &response) const
{
	std::cout << "==RESPONSE==" << std::endl;
	std::cout << response << std::endl;
	std::cout << "==end==" << std::endl;
	if(send(_acceptFd, response.c_str(), response.size(), 0) == -1)
	{
		std::cerr << "send error" << std::endl;
		throw server::syscallErrorException();
	}
}

void 					server::serverClose()
{
	if(_acceptFd > 0)
		::close(_acceptFd);
	_acceptFd = -1;
}

void 	server::accept()
{
	struct sockaddr connectingAddr;
	socklen_t		addressLen;
	this->_acceptFd = ::accept(this->_socketFd, &connectingAddr, &addressLen);
	if (_acceptFd == -1)
		std::cerr << "Could not create fd" << std::endl; // dit zometeen aanpassen naar try catch
}

void	server::run()
{
	std::string receivedRequest;
	this->accept();
	try
	{
		receivedRequest = receive();
	}
	catch (std::exception &e)
	{
		// error
	}

	Request	request(receivedRequest);
	Response resp(request, *this);
	resp.setupResponse(request, *this);
	try
	{
		this->sendData(resp.getResponse());
	}
	catch (std::exception &e)
	{
		// error
	}
	this->serverClose();
}
