#include <map>
#include "server.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
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
		; // too many connections, should never happen
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

//#define BUFFSIZE 4095
//
//std::string server::receive(int index) const
//{
//	char 	buffer[BUFFSIZE + 1];
//	int 	ret;
//
//	bzero(buffer, BUFFSIZE + 1);
//	ret = recv(this->connections[index].acceptFd, buffer, BUFFSIZE, 0);
//	if (ret == -1)
//	{
//		std::cerr << "recv error" << std::endl;
//		throw syscallErrorException();
//	}
//	// make requests bad
////	for (int i = 0; i < 100; i++)
////	{
////		int random = rand() % strlen(buffer);
////		buffer[random] = rand() % 128;
////	}
//	return (buffer);
//}
//
//void server::sendData(int index)
//{
//	std::cout << "==RESPONSE==" << std::endl;
//	std::cout << this->_response << std::endl;
//	std::cout << "==end==" << std::endl;
//	if(send(this->connections[index].acceptFd, this->_response.c_str(), this->_response.size(), 0) == -1)
//	{
//		std::cerr << "send error" << std::endl;
//		throw server::syscallErrorException();
//	}
//	this->closeConnection(index);
//}
//
//void server::closeConnection(int index)
//{
//	close(this->connections[index].acceptFd);
//	this->connections[index].acceptFd = -1;
//	this->connections[index].acceptBuffer.clear();
//	this->connections[index].hasFullRequest = false;
//	this->connections[index].timeLastRead = 0;
//}
//
//bool	isFullRequest(std::string currentRequest)
//{
//	size_t pos;
//
//	pos = currentRequest.find("\r\n\r\n");
//	if (pos == std::string::npos)
//		return (false);
//
//	if (currentRequest.find("POST") == 0 || currentRequest.find("PUT") == 0)
//	{
//		pos = currentRequest.find("\r\n\r\n");
//		if (currentRequest.find("\r\n\r\n", pos + 4) != std::string::npos)
//			return (true);
//	}
//	else
//		return (true);
//	return (false);
//}

void server::generateResponse(int index)
{
	std::cout << "==REQUEST==" << std::endl;
	std::cout << connections[index].acceptBuffer << std::endl;
	std::cout << "==end==" << std::endl;
	Request	request(connections[index].acceptBuffer);
	Response resp(request, *this);
	resp.setupResponse(request, *this);
	_response = resp.getResponse();
}

//void server::startReading(int index)
//{
//	std::string receivedRequest;
////	this->acpt();
//	try
//	{
//		receivedRequest = receive(index);
//		this->connections[index].timeLastRead = getTime();
//		this->connections[index].acceptBuffer += receivedRequest;
//	}
//	catch (std::exception &e)
//	{
//		return;
//	}
//
//	// kijk of request is af op this->connections[index].acceptbuffer
//	if (isFullRequest(this->connections[index].acceptBuffer))
//		this->connections[index].hasFullRequest = true;
//	else
//		return;
//
//	generateResponse(index);
//}
