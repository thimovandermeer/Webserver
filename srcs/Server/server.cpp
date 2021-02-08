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

const std::vector<location>		&server::getLocations() const
{
	return (this->_locations);
}

const int	&server::getSocketFd() const
{
	return (this->_socketFd);
}

const struct sockaddr_in	&server::getAddr() const
{
	return (this->_addr);
}

const int	&server::getAcceptFd() const
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

void	server::addLocation(location &newLoc)
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

std::string 		server::receive() const
{
	char		buffer[4096];
	std::string request;
	int 		read = 4095;

	while( read == 4095)
	{
		memset(buffer, 0, 4096);
		read = recv(_acceptFd, buffer, 4095, 0);
		if (read == -1)
		{
			std::cerr << "recv error" << std::endl;
			throw server::syscallErrorException();
		}
		request += std::string(buffer);
	}
	std::cout << request << std::endl;
	return (request);
}

void 		server::sendData(const std::string &response) const
{
	std::cout << response << std::endl;
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
	response.checkMethod(request, *this);
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

