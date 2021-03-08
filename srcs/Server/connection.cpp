#include "connection.hpp"
#include "../Utils/utils.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

connection::connection() : timeLastRead(0), acceptFd(-1), hasFullRequest(false)
{
}

connection::~connection()
{
	close(this->acceptFd);
}

connection::connection(const connection &original)
{
	*this = original;
}

connection &connection::operator=(const connection &original)
{
	this->timeLastRead = original.timeLastRead;
	this->acceptFd = original.acceptFd;
	this->acceptBuffer = original.acceptBuffer;
	this->hasFullRequest = original.hasFullRequest;
	return (*this);
}

void	connection::closeThisConnection()
{
	close(this->acceptFd);
	this->acceptFd = -1;
	this->acceptBuffer.clear();
	this->hasFullRequest = false;
	this->timeLastRead = 0;
}

void	connection::sendData(std::string &response)
{
	std::cout << "==RESPONSE==" << std::endl;
	std::cout << response << std::endl;
	std::cout << "==end==" << std::endl;
	if(send(this->acceptFd, response.c_str(), response.size(), 0) == -1)
	{
		std::cerr << "send error" << std::endl;
	}
	this->closeThisConnection();
}

std::string	connection::receive() const
{
	char 	buffer[BUFFSIZE + 1];
	int 	ret;

	bzero(buffer, BUFFSIZE + 1);
	ret = recv(this->acceptFd, buffer, BUFFSIZE, 0);
	if (ret == -1)
	{
		std::cerr << "recv error" << std::endl;
//		throw syscallErrorException();
	}
	// make requests bad
//	for (int i = 0; i < 100; i++)
//	{
//		int random = rand() % strlen(buffer);
//		buffer[random] = rand() % 128;
//	}
	return (buffer);
}

void	connection::startReading()
{
	std::string receivedRequest;
//	this->acpt();
	try
	{
		receivedRequest = receive();
		this->timeLastRead = getTime();
		this->acceptBuffer += receivedRequest;
	}
	catch (std::exception &e)
	{
		return;
	}

	// kijk of request is af op this->acceptbuffer
	if (isFullRequest(this->acceptBuffer))
		this->hasFullRequest = true;
	else
		return;

//	generateResponse(*this);
}

bool	connection::isFullRequest(std::string &currentRequest) const
{
	size_t pos;

	pos = currentRequest.find("\r\n\r\n");
	if (pos == std::string::npos)
		return (false);

	if (currentRequest.find("POST") == 0 || currentRequest.find("PUT") == 0)
	{
		pos = currentRequest.find("\r\n\r\n");
		if (currentRequest.find("\r\n\r\n", pos + 4) != std::string::npos)
			return (true);
	}
	else
		return (true);
	return (false);
}


