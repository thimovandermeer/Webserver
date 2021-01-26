#include "server.hpp"
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>

const char	*server::inputErrorException::what() const throw()
{
	return ("config file is incorrect");
}

server::server() : _portNr(0), _maxBodySize(1000000), _autoindex(false)
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
{}

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

int			server::getPortNr() const
{
	return (this->_portNr);
}

size_t		server::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

bool		server::getAutoindex() const
{
	return (this->_autoindex);
}

std::string	server::getRoot() const
{
	return (this->_root);
}

std::string	server::getErrorPage() const
{
	return (this->_errorPage);
}

std::string	server::getHost() const
{
	return (this->_host);
}

std::vector<std::string>	server::getServerNames() const
{
	return (this->_serverNames);
}

std::vector<std::string>	server::getIndices() const
{
	return (this->_indices);
}

bool	server::valueCheck() const
{
	if (this->_portNr <= 0)
		return (false);
	if (this->_maxBodySize <= 0)
		return (false);
	if (this->_errorPage.empty())
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

std::ostream&	operator<<(std::ostream& os, const server& serv)
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
	return (os);
}