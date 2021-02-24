#include <map>
#include "server.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
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

int doneReading(std::string request, int type)
{
	int         pos1;
	int         pos2;
	std::string length;
	char*        subStr;
	int         contentLen;
	char        *end;

	if (type == 1 && request.find("\r\n\r\n0\r\n\r\n") != std::string::npos)        //of is dit too much
		return 1;
	else if (type == 2) {
		pos1 = request.find("Content-Length");      //opzoeken waar de lengte staat
		pos2 = request.find(":", pos1);             //waar de lengte begint
//        pos1 = request.find("\r\n", pos2);          //kijken tot waar de index van de lengte loopt
//        subStr = (char*)request.substr(pos1);

		contentLen = std::strtol(subStr, &end ,10);
//        length = request.substr(pos2, pos1 - pos2);     // de lengte eruit halen
//        contentLen = std::strtol(length.c_str());           //de lengte ophalen //mag je strol gebruiken?
		pos1 = request.find("\r\n\r\n");            //zoeken naar het einde van alle headers
		pos2 = request.length();                //kijken hoe lang de gehele request is
		if (contentLen == pos2 - pos1)      //als dit gelijk is, is alles dus gelezen
			return 1;
	}
	return 0;
}

#define BUFFSIZE 4095
std::string 		server::receive() const
{
	char		buffer[BUFFSIZE + 1];
	std::string request;
	int 		ret = 0;
	int         type;

	while(true)
	{
		std::cout << "reading..." << std::endl;
		ft_bzero(buffer, ret);
		ret = read(_acceptFd, buffer, BUFFSIZE);
		if (ret == -1)
		{
			std::cerr << "recv error" << std::endl;
			throw server::syscallErrorException();
		}
		buffer[ret] = 0;
		request += std::string(buffer);
		// als hij geen body heeft en \r\n\r\n heeft gevonden is hij klaar met lezen
		if(!(type = hasBody(request)) && request.find("\r\n\r\n") != std::string::npos)
			break;
		// als er wel een body is, gaan we checken of alles daaruit is gelezen
		if (doneReading(request, type))
			break;
		else
			continue;
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

void 		server::serverClose()
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
	//	fcntl(this->_acceptFd, F_SETFL, O_NONBLOCK);
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
		return;
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
