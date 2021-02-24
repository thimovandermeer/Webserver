#include "location.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <dns_sd.h>
#include "../Utils/utils.hpp"
#include "../Utils/Base64.hpp"

const char	*location::inputErrorException::what() const throw()
{
	return ("error in location block in config file");
}

location::location(std::string &match) : _autoindex(false), _isFileExtension(false)
{
	this->_match = match;
	if (match[0] == '*' && match[1] == '.')
		this->_isFileExtension = true;
	this->_typeFunctionMap.insert(std::make_pair("autoindex", &location::setAutoindex));
	this->_typeFunctionMap.insert(std::make_pair("root", &location::setRoot));
	this->_typeFunctionMap.insert(std::make_pair("method", &location::setMethod));
	this->_typeFunctionMap.insert(std::make_pair("error_page", &location::setErrorPage));
	this->_typeFunctionMap.insert(std::make_pair("index", &location::setIndices));
	this->_typeFunctionMap.insert(std::make_pair("cgi_exec", &location::setCgiPath));
	this->_typeFunctionMap.insert(std::make_pair("auth_basic", &location::setAuthBasic));
	this->_typeFunctionMap.insert(std::make_pair("auth_basic_user_file", &location::sethtpasswdpath));
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
	this->_typeFunctionMap = original._typeFunctionMap;
	this->_authBasic = original._authBasic;
	this->_authBasicUserFile = original._authBasicUserFile;
	this->_cgiPath = original._cgiPath;
	this->_isFileExtension = original._isFileExtension;
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

void	location::setCgiPath(std::string &cgiPass)
{
	this->_cgiPath = cgiPass;
}

void	location::setAuthBasic(std::string &authBasic)
{
	this->_authBasic = authBasic;
}

void	location::setAuthUserFile(std::string &userFile)
{
	this->_authBasicUserFile = userFile;
}

void 	location::sethtpasswdpath(std::string &path)
{
	struct stat statstruct = {};
	if (stat(path.c_str(), &statstruct) == -1)
		return ;

	this->_htpasswd_path = path;
	int htpasswd_fd = open(this->_htpasswd_path.c_str(), O_RDONLY);
	if (htpasswd_fd < 0)
		return;
	std::string line;
	while (get_next_line(htpasswd_fd, line)) {
		std::string user;
		std::string pass;
		get_key_value(line, user, pass, ":", "\n\r#;");
		this->_loginfo[user] = pass;
	}
	if (close(htpasswd_fd) == -1)
		throw std::runtime_error("Failed to close .htpasswd file");
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

const std::string				&location::getCgiPath() const
{
	return (this->_cgiPath);
}

const std::string				&location::getAuthBasic() const
{
	return (this->_authBasic);
}

const std::string				&location::getAuthUserFile() const
{
	return (this->_authBasicUserFile);
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

bool	location::isFileExtension() const
{
	return (this->_isFileExtension);
}

bool location::getMatch(const std::string& username, const std::string& passwd)
{
	std::map<std::string, std::string>::const_iterator it = this->_loginfo.find(username);

	return ( it != _loginfo.end() && passwd == base64_decode(it->second) );
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