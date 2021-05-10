#include <map>
#include "server.hpp"
#include "location.hpp"

const char	*server::inputErrorException::what() const throw()
{
	return ("error in server block in config file");
}

const char	*server::syscallErrorException::what() const throw()
{
	return ("a syscall has returned an error");
}

server::server() : _portNr(0), _maxBodySize(1000000), _autoindex(false), _errorPage("default_error_page"), _socketFd(-1)
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
	this->_maxBodySize = original._maxBodySize;
	this->_autoindex = original._autoindex;
	this->_root = original._root;
	this->_errorPage = original._errorPage;
	this->_host = original._host;
	this->_serverNames = original._serverNames;
	this->_indices = original._indices;
	this->_typeFunctionMap = original._typeFunctionMap;
	this->_locations = original._locations;
	this->_socketFd = original._socketFd;
	this->_addr = original._addr;
	return (*this);
}

void	server::setPort(std::string &portNr)
{
	std::stringstream	ss(portNr);
	ss >> this->_portNr;
}

void	server::setMaxBodySize(std::string &size)
{
	std::stringstream	ss(size);
	ss >> this->_maxBodySize;
	if (this->_maxBodySize == 0) 
		this->_maxBodySize = (ULONG_MAX); 
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

void    server::setAlternativeServers(server *alternative) {
    this->_alternativeServers.push_back((alternative));
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

bool	server::valueCheck() const
{
	if (this->_portNr <= 0)
		return (false);
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
	os << std::setw(15) << std::left << "socket fd: " << serv.getSocketFd() << std::endl;

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
