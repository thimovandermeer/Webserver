#include <map>
#include "server.hpp"
#include "location.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/socket.h>
#include "../Request/request.hpp"
#include "../Response/Response.hpp"

const char	*server::inputErrorException::what() const throw()
{
	return ("error in server block in config file");
}

const char	*server::syscallErrorException::what() const throw()
{
	return ("a syscall has returned an error");
}

server::server() : _portNr(0), _maxBodySize(1000000), _autoindex(false), _errorPage("default_error_page"), _socketFd(-1), _acceptFd(-1) //should change default error page
{
	this->_typeFunctionMap.insert(std::make_pair("listen", &server::setPort));
	this->_typeFunctionMap.insert(std::make_pair("client_max_body_size", &server::setMaxBodySize));
	this->_typeFunctionMap.insert(std::make_pair("autoindex", &server::setAutoindex));
	this->_typeFunctionMap.insert(std::make_pair("root", &server::setRoot));
	this->_typeFunctionMap.insert(std::make_pair("error_page", &server::setErrorPage));
	this->_typeFunctionMap.insert(std::make_pair("host", &server::setHost));
	this->_typeFunctionMap.insert(std::make_pair("server_name", &server::setServerNames));
	this->_typeFunctionMap.insert(std::make_pair("index", &server::setIndices));
}

server::server(server const &original) : _portNr(), _maxBodySize(), _autoindex()
{
	*this = original;
}

server::~server()
{
	close(this->_socketFd);
	close(this->_acceptFd);
	std::vector<location*>::iterator it;
	it = this->_locations.begin();
	while (!this->_locations.empty() && it != this->_locations.end())
	{
		delete (*it);
		it++;
	}
}

server&	server::operator=(server const &original)
{
	this->_portNr = original._portNr;
	this->_root = original._root;
	this->_serverNames = original._serverNames;
	this->_errorPage = original._errorPage;
	this->_maxBodySize = original._maxBodySize;
	this->_indices = original._indices;
	this->_autoindex = original._autoindex;
	this->_host = original._host;
	this->_acceptFd = original._acceptFd;
	this->_socketFd = original._socketFd;
	this->_addr = original._addr;
	return (*this);
}

void	server::setPort(std::string &portNr)
{
//	this->_portNr = stoi(portNr); this is C++11
	std::stringstream	ss(portNr);
	ss >> this->_portNr;
}

void	server::setMaxBodySize(std::string &size)
{
//	this->_maxBodySize = stol(size); this is C++11
	std::stringstream	ss(size);
	ss >> this->_maxBodySize;
	if (this->_maxBodySize == 0) // unlimited
		this->_maxBodySize = (ULLONG_MAX); // this is 18.45 million terrabyte, I think we're ok with 'unlimited'
}

void	server::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
	{
		this->_autoindex = true;
		return;
	}
	if (autoindex != "off") // input is neither 'on' nor 'off', so wrong
		throw server::inputErrorException();
}

void	server::setRoot(std::string &root)
{
	this->_root = root;
}

void	server::setErrorPage(std::string &page)
{
	this->_errorPage = page;
}

void	server::setHost(std::string &host)
{
	this->_host = host;
}

void	server::setServerNames(std::string &names)
{
	std::stringstream	ss(names);
	std::string			name;
	while (ss >> name)
		this->_serverNames.push_back(name);
}

void	server::setIndices(std::string &indices)
{
	std::stringstream	ss(indices);
	std::string			index;
	while (ss >> index)
		this->_indices.push_back(index);
}

void	server::setAcceptFd(int fd)
{
	this->_acceptFd = fd;
}

const int			&server::getPortNr() const
{
	return (this->_portNr);
}

const size_t		&server::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

const bool			&server::getAutoindex() const
{
	return (this->_autoindex);
}

const std::string	&server::getRoot() const
{
	return (this->_root);
}

const std::string	&server::getErrorPage() const
{
	return (this->_errorPage);
}

const std::string	&server::getHost() const
{
	return (this->_host);
}

const std::vector<std::string>	&server::getServerNames() const
{
	return (this->_serverNames);
}

const std::vector<std::string>	&server::getIndices() const
{
	return (this->_indices);
}

const std::vector<location*>		&server::getLocations() const
{
	return (this->_locations);
}

const long	&server::getSocketFd() const
{
	return (this->_socketFd);
}

const struct sockaddr_in	&server::getAddr() const
{
	return (this->_addr);
}

const long	&server::getAcceptFd() const
{
	return(this->_acceptFd);
}

bool	server::valueCheck() const
{
	if (this->_portNr <= 0)
		return (false);
//	if (this->_errorPage.empty())
//		return (false);
	if (this->_host.empty())
		return (false);
	return (true);

}

void	server::findValue(std::string &key, std::string line)
{
	if (*(line.rbegin()) != ';') // line doesn't end with ';'
		throw server::inputErrorException();

	std::map<std::string, setter>::iterator it;
	it = this->_typeFunctionMap.find(key);
	if (it == this->_typeFunctionMap.end()) // unknown key value
	{
		std::cerr << "unknown key: " << key << std::endl;
		throw server::inputErrorException();
	}
	line.resize(line.size() - 1); // remove ';'
	line = line.substr(line.find_first_of(" \t") + 1); // remove first word
	(this->*(this->_typeFunctionMap.at(key)))(line);
}

void	server::addLocation(location *newLoc)
{
	this->_locations.push_back(newLoc);
}

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


int doneReading(std::string request, int type)
{
    int         pos1;
    int         pos2;
    std::string length;
    char*        subStr;
    int         contentLen;
    char        *end;

    if (type == 1 && request.find("\r\n0\r\n") != std::string::npos)
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
		buffer[ret] = 0;
		if (ret == -1)
		{
			std::cerr << "recv error" << std::endl;
			throw server::syscallErrorException();
		}
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
	Response response;

	Request	request(receivedRequest);
	response.setupResponse(request, *this);
	try
	{
		this->sendData(response.getResponse());
	}
	catch (std::exception &e)
	{
		// error
	}
	this->serverClose();
}

location*		server::findLocation(std::string &match)
{
	std::vector<location*>::iterator it;
	it = this->_locations.begin();
	while (!this->_locations.empty() && it != this->_locations.end())
	{
		if ((*it)->getMatch() == match)
			return (*it);
		it++;
	}
	return (NULL);
}

std::ostream&	operator<<(std::ostream &os, const server &serv)
{
	os << std::setw(15) << std::left << "portNr: " << serv.getPortNr() << std::endl;
	os << std::setw(15) << std::left << "max body size: " << serv.getMaxBodySize() << std::endl;
	os << std::setw(15) << std::left << "autoindex: ";
	if (serv.getAutoindex())
		os << "on" << std::endl;
	else
		os << "off" << std::endl;
	os << std::setw(15) << std::left << "root: " << serv.getRoot() << std::endl;
	os << std::setw(15) << std::left << "errorpage: " << serv.getErrorPage() << std::endl;
	os << std::setw(15) << std::left << "host: " << serv.getHost() << std::endl;

	std::vector<std::string> vc;
	std::vector<std::string>::iterator it;

	vc = serv.getServerNames();
	it = vc.begin();
	os << "server names: " << std::endl;
	while (!vc.empty() && it != vc.end())
	{
		os << "\t- " << *it << std::endl;
		it++;
	}
	vc = serv.getIndices();
	it = vc.begin();
	os << "indices: " << std::endl;
	while (!vc.empty() && it != vc.end())
	{
		os << "\t- " << *it << std::endl;
		it++;
	}
	os << "locations: " << std::endl;
	for (size_t i = 0; i < serv.getLocations().size(); i++)
		os << " >" << serv.getLocations()[i] << std::endl;
	return (os);
}

