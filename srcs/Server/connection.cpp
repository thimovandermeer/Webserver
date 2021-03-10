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

#include <algorithm>
#include <sstream>
void connection::sendData(std::string &response, const size_t bodylen)
{
	std::cout << "==RESPONSE==" << std::endl;
	int len = std::min(response.length(), (size_t)500);
	if (write(1, response.c_str(), len) == -1) {;}
	std::cout << "\n==end==" << std::endl;

	size_t headerlen = response.find("\r\n\r\n") + 4;

	if (bodylen < (size_t)MAXSENDSIZE)
	{
		if (send(this->acceptFd, response.c_str(), response.length(), 0) == -1)
		{
			std::cerr << "send error" << std::endl;
			std::cerr << "errno is " << errno << std::endl;
			std::cerr << strerror(errno) << std::endl;
		}
	}
	else // chunked response
	{
		// send header
		std::string headr = response.substr(0, headerlen);
//		std::cerr << "sending header" << std::endl;
		send(this->acceptFd, headr.c_str(), headr.length(), 0);
		size_t bodyBytesSent = 0;
		while(bodylen - bodyBytesSent > 0)
		{
//			std::cerr << "sending chunk" << std::endl;
			size_t	bytesToSend = std::min((bodylen - bodyBytesSent), (size_t)MAXSENDSIZE);

			std::stringstream ss;

			ss << std::hex << bytesToSend;
			std::string chunk(ss.str());
			chunk += "\r\n";
			chunk.append(response, headerlen + bodyBytesSent, bytesToSend);
			chunk += "\r\n";
			send(this->acceptFd, chunk.c_str(), chunk.length(), 0);
//			std::cerr << "done sending chunk" << std::endl;
			bodyBytesSent += bytesToSend;
			// repeat
		}
		std::string end("0\r\n\r\n");
		send(this->acceptFd, end.c_str(), 5, 0);
	}
	this->closeThisConnection();
}

std::string	connection::receive() const
{
	char 	buffer[MAXREADSIZE + 1];
	int 	ret;

	bzero(buffer, MAXREADSIZE + 1);
	ret = recv(this->acceptFd, buffer, MAXREADSIZE, 0);
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

	if (isFullRequest(this->acceptBuffer))
		this->hasFullRequest = true;
	else
		return;
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
