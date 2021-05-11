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

#include <fstream>
static size_t nr = 0;
void server::createResponse(int index)
{
	connection	*curcon = &this->connections[index];
	std::cout << "handling request nr " << nr << std::endl;

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
	if (!(*this)._alternativeServers.empty()) {
		if ((*this)._serverNames[0] != request.getHost()) {
			std::vector<server *>::const_iterator it1;
			for (it1 = (*this)._alternativeServers.begin(); it1 != (*this)._alternativeServers.end(); it1++) {
				if ((*it1)->_serverNames[0] == request.getHost()){
					*this = *(*it1);
					break ;
				}
			}
		}
	}
	curcon->myresp = new Response(request, *this);
	curcon->myresp->setupResponse(request, *this);

	nr++;
}

void	server::setupRespStr(int index)
{
	connection	*curcon = &this->connections[index];

	if (!curcon->myresp->isRespFinished())
	{
		if (curcon->myresp->getUseCgi())
			curcon->myresp->getCgi().executeGCI(curcon->myresp->getBody());
		if (curcon->myresp->getStatus() > 299)
			curcon->myresp->finishErrorPage(*this);
		else if (curcon->myresp->methodType() == "GET")
			curcon->myresp->getMethod();
		else if (curcon->myresp->methodType() == "HEAD")
			curcon->myresp->headMethod();
		else if (curcon->myresp->methodType() == "POST")
			curcon->myresp->finishPost();
		else if (curcon->myresp->methodType() == "PUT")
			curcon->myresp->finishPut();
	}
	this->_bodylen = curcon->myresp->getBodySize();
	curcon->setResponseString(curcon->myresp->getResponse());

#ifdef PRINTLOG
	if (nr >= MAXLOGS)
	{
		std::stringstream oldname;
		oldname << "logs/response_";
		size_t oldnr = nr - MAXLOGS;
		oldname << oldnr;
		remove(oldname.str().c_str());
	}
	std::stringstream logname1;
	logname1 << "logs/response_";
	logname1 << nr;
	std::ofstream respLog(logname1.str().c_str(), std::ios::out);
	respLog << curcon->getResponseString();
	respLog.close();
#endif
#ifdef PRINTOUT
	std::cout << "==RESPONSE==" << std::endl;
	int len1 = std::min(curcon->getResponseString().length(), (size_t)500);
	if (write(1, curcon->getResponseString().c_str(), len1) == -1) {;}
	std::cout << "\n==end==" << std::endl;
#endif
}
