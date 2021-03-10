#include "connection.hpp"
#include "../Utils/utils.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <cstring>

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

#define SENDSIZE 524288
void	connection::sendData(std::string &response)
{
	std::cout << "==RESPONSE==" << std::endl;
	int len = response.length() > 500 ? 500 : response.length();
	if (write(1, response.c_str(), len) == -1) {;}
	std::cout << "\n==end==" << std::endl;

	if (response.size() < SENDSIZE)
	{
		if (send(this->acceptFd, response.c_str(), response.size(), 0) == -1)
		{
			std::cerr << "size is" << response.size() << std::endl;
			std::cerr << "send error" << std::endl;
		}
	}
	else
	{
		const char *str = response.c_str();
		size_t sizeLeft = response.size();
		size_t sizeSent = 0;
		while (sizeLeft > 0)
		{
			size_t thisSend = sizeLeft > SENDSIZE ? SENDSIZE : sizeLeft;
			if (send(this->acceptFd, str + sizeSent, thisSend, 0) == -1)
			{
				std::cerr << "this is really uncool man\nsize is " << thisSend << std::endl;
			}
			else
			{
				sizeLeft -= thisSend;
				sizeSent += thisSend;
			}
		}
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


