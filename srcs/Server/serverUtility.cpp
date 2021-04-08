#include "server.hpp"
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
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int	ret;
	int options = 1;
	ret = setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEPORT, &options, sizeof(options));
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

int server::acpt()
{
	struct sockaddr connectingAddr;
	socklen_t		addressLen;
	int				i;

	for (i = 0; i < NR_OF_CONNECTIONS; i++)
	{
		if (this->connections[i].getAcceptFd() == -1)
			break;
	}
	if (i == NR_OF_CONNECTIONS)
	{
		return (0);
	}
	this->connections[i].setFd(accept(this->_socketFd, &connectingAddr, &addressLen));
	if (this->connections[i].getAcceptFd() == -1)
		std::cerr << "Could not create fd" << std::endl;
	this->connections[i].setTimeLastRead(getTime());
	return (1);
}

#include <sstream>

void server::generateResponse(int index)
{
	static size_t nr = 0;
	if (!this->connections[index].getResponseString().empty())
		return;
	std::cout << "handling request nr " << nr << std::endl;
	system("leaks webserv");

#ifdef PRINTLOG
	if (nr >= MAXLOGS)
	{
		std::stringstream oldname;
		oldname << "logs/request_";
		size_t oldnr = nr - MAXLOGS;
		oldname << oldnr;
		remove(oldname.str().c_str());
	}
	std::stringstream logname;
	logname << "logs/request_";
	logname << nr;
	std::ofstream reqLog(logname.str().c_str(), std::ios::out);
	reqLog << connections[index].getBuffer();
	reqLog.close();
#endif
#ifdef PRINTOUT
	std::cout << "==REQUEST==" << std::endl;
	int len = std::min(connections[index].getBuffer().length(), (size_t)500);
	if (write(1, connections[index].getBuffer().c_str(), len) == -1) {;}
	std::cout << "==end==" << std::endl;
#endif
	Request	request(this->connections[index].getBuffer());
	Response resp(request, *this);
	resp.setupResponse(request, *this);
	this->_bodylen = resp.getBodySize();
	this->connections[index].setResponseString(resp.getResponse());

#ifdef PRINTLOG
	if (nr >= MAXLOGS)
	{
		std::stringstream oldname;
		oldname << "logs/response_";
		size_t oldnr = nr - MAXLOGS;
		oldname << oldnr;
		remove(oldname.str().c_str());
	}
	std::stringstream logname;
	logname << "logs/response_";
	logname << nr;
	std::ofstream respLog(logname.str().c_str(), std::ios::out);
	respLog << this->connections[index].getResponseString();
	respLog.close();
#endif
#ifdef PRINTOUT
	std::cout << "==RESPONSE==" << std::endl;
	int len1 = std::min(this->connections[index].getResponseString().length(), (size_t)500);
	if (write(1, this->connections[index].getResponseString().c_str(), len1) == -1) {;}
	std::cout << "\n==end==" << std::endl;
#endif
	nr++;
}
