#include "server.hpp"

server::server() : _portNr(0), _maxBodySize(0), _autoindex(false)
{}

server::server(server const &original)
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

void	server::setPort(int portNr)
{
	this->_portNr = portNr;
}

void	server::setmaxBodySize(size_t size)
{
	this->_maxBodySize = size;
}

void	server::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
	{
		this->_autoindex = true;
		return;
	}
	if (autoindex != "off") // input is wrong
		;
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
	if (this->_serverNames.empty())
		return (false);
	return (true);

}
