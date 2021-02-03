#include "location.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>

const char	*location::inputErrorException::what() const throw()
{
	return ("error in location block in config file");
}

location::location(std::string &match) : _autoindex(false)
{
	this->_match = match;
	this->_typeFunctionMap.insert(std::make_pair("autoindex", &location::setAutoindex));
	this->_typeFunctionMap.insert(std::make_pair("root", &location::setRoot));
	this->_typeFunctionMap.insert(std::make_pair("method", &location::setMethod));
	this->_typeFunctionMap.insert(std::make_pair("error_page", &location::setErrorPage));
	this->_typeFunctionMap.insert(std::make_pair("index", &location::setIndices));
}

location::location(const location &original)
{
	*this = original;
}

location::~location()
{}

location	&location::operator=(const location &original)
{
	this->_autoindex = original._autoindex;
	this->_match = original._match;
	this->_root = original._root;
	this->_method = original._method;
	this->_errorPage = original._errorPage;
	this->_indices = original._indices;
	return (*this);
}

void	location::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
	{
		this->_autoindex = true;
		return;
	}
	if (autoindex != "off") // input is neither 'on' nor 'off', so wrong
		;
}

//void	location::setMatch(std::string &match)
//{
//		this->_match = match;
//}

void	location::setRoot(std::string &root)
{
	this->_root = root;
}

void	location::setMethod(std::string &method)
{
	this->_method = method;
}

void	location::setErrorPage(std::string &errorPage)
{
	this->_errorPage = errorPage;
}

void	location::setIndices(std::string &indices)
{
	std::stringstream	ss(indices);
	std::string			index;
	while (ss >> index)
		this->_indices.push_back(index);
}

const bool						&location::getAutoindex() const
{
	return (this->_autoindex);
}

const std::string				&location::getMatch() const
{
	return (this->_match);
}

const std::string				&location::getRoot() const
{
	return (this->_root);
}

const std::string				&location::getMethod() const
{
	return (this->_method);
}

const std::string				&location::getErrorPage() const
{
	return (this->_errorPage);
}

const std::vector<std::string>	&location::getIndices() const
{
	return (this->_indices);
}

void	location::findValue(std::string &key, std::string line)
{
	if (*(line.rbegin()) != ';') // line doesn't end with ';'
		throw location::inputErrorException();

	std::map<std::string, setter>::iterator it;
	it = this->_typeFunctionMap.find(key);
	if (it == this->_typeFunctionMap.end()) // unknown key value
	{
		std::cerr << "unknown key: " << key << std::endl;
		throw location::inputErrorException();
	}
	line.resize(line.size() - 1); // remove ';'
	line = line.substr(line.find_first_of(" \t") + 1); // remove first word
	(this->*(this->_typeFunctionMap.at(key)))(line);
}

bool	location::valueCheck() const
{
	std::string	allowedMethods[5] = {"", "HEAD", "GET", "POST", "PUT"};
	bool	ret = false;
	for (int i = 0; i < 5; i++)
	{
		if (this->_method == allowedMethods[i])
			ret = true;
	}
	if (!ret)
		return (ret);
	// can test for more stuff here, don't know what though
	return (true);
}

std::ostream&	operator<<(std::ostream &os, const location &loc)
{
	os << std::setw(13) << std::left << "\tmatch: " << loc.getMatch() << std::endl;
	os << std::setw(13) << std::left << "\tautoindex: ";
	if (loc.getAutoindex())
		os << "on" << std::endl;
	else
		os << "off" << std::endl;
	os << std::setw(13) << std::left << "\troot: " << loc.getRoot() <<std::endl;
	os << std::setw(13) << std::left << "\tmethod: " << loc.getMethod() <<std::endl;
	os << std::setw(13) << std::left << "\terror page: " << loc.getErrorPage() <<std::endl;

	std::vector<std::string> vc;
	std::vector<std::string>::iterator it;
	vc = loc.getIndices();
	it = vc.begin();

	os << std::setw(13) << std::left << "\tindices: " <<std::endl;
	while (!vc.empty() && it != vc.end())
	{
		os << "\t\t- " << *it << std::endl;
		it++;
	}
	return (os);
}