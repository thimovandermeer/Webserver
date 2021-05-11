#include "location.hpp"
#include <fstream>

const char	*location::inputErrorException::what() const throw()
{
	return ("error in location block in config file");
}

location::location(std::string &match) : _autoindex(false),  _ownAutoindex(false),
										_maxBodySize(1000000), _ownBodySize(false),
										_isFileExtension(false)
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
	this->_typeFunctionMap.insert(std::make_pair("client_max_body_size", &location::setMaxBody));
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
	this->_methods = original._methods;
	this->_errorPage = original._errorPage;
	this->_indices = original._indices;
	this->_typeFunctionMap = original._typeFunctionMap;
	this->_authBasic = original._authBasic;
	this->_authBasicUserFile = original._authBasicUserFile;
	this->_cgiPath = original._cgiPath;
	this->_isFileExtension = original._isFileExtension;
	return (*this);
}

void	location::setAutoindex(const std::string &autoindex)
{
	this->_ownAutoindex = true;
	if (autoindex == "on")
	{
		this->_autoindex = true;
		return;
	}
	if (autoindex != "off")
	{;}
}

void		location::setMaxBody(const std::string &size)
{
	this->_ownBodySize = true;
	std::stringstream	ss(size);
	ss >> this->_maxBodySize;
	if (this->_maxBodySize == 0) 
		this->_maxBodySize = (ULONG_MAX); 
}

void	location::setRoot(const std::string &root)
{
	this->_root = root;
}

void	location::setMethod(const std::string &method)
{
	std::stringstream	ss(method);
	std::string			meth;
	while (ss >> meth)
		this->_methods.push_back(meth);
}

void	location::setErrorPage(const std::string &errorPage)
{
	this->_errorPage = errorPage;
}

void	location::setIndices(const std::string &indices)
{
	std::stringstream	ss(indices);
	std::string			index;
	while (ss >> index)
		this->_indices.push_back(index);
}

void	location::setCgiPath(const std::string &cgiPass)
{
	this->_cgiPath = cgiPass;
}

void	location::setAuthBasic(const std::string &authBasic)
{
	this->_authBasic = authBasic;
}

void 	location::sethtpasswdpath(const std::string &path)
{
	struct stat	statstruct = {};
	if (stat(path.c_str(), &statstruct) == -1)
		return ;
	this->_htpasswd_path = path;
	std::fstream	configfile;
	std::string line;

	configfile.open(this->_htpasswd_path.c_str());
	if (!configfile)
		return;
	while (std::getline(configfile, line)) {
		std::string user;
		std::string pass;
		get_key_value(line, user, pass, ":", "\n\r#;");
		this->_loginfo[user] = pass;
	}
	configfile.close();
}

const bool						&location::hasOwnAutoindex() const
{
	return (this->_ownAutoindex);
}

const bool						&location::hasOwnBodySize() const
{
	return (this->_ownBodySize);
}

const bool						&location::getAutoindex() const
{
	return (this->_autoindex);
}

const size_t					&location::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

const std::string				&location::getMatch() const
{
	return (this->_match);
}

const std::string				&location::getRoot() const
{
	return (this->_root);
}

const std::vector<std::string>	&location::getMethods() const
{
	return (this->_methods);
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

std::string	location::gethtpasswdpath() const {
	return _htpasswd_path;
}

void	location::findValue(std::string &key, std::string line)
{
	std::map<std::string, setter>::iterator it;

	if (*(line.rbegin()) != ';')
		throw location::inputErrorException();
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
	bool	ret = true;

	std::vector<std::string>::const_iterator it;
	for (it = this->_methods.begin(); it < this->_methods.end(); it++)
	{
		if ((*it) != allowedMethods[0] && (*it) != allowedMethods[1] && (*it) != allowedMethods[2] && (*it) != allowedMethods[3] && (*it) != allowedMethods[4])
			ret = false;
	}
	if (!ret)
		return (ret);
	return (true);
}

bool	location::isFileExtension() const
{
	return (this->_isFileExtension);
}

bool location::getAuthMatch(const std::string& username, const std::string& passwd)
{
	std::map<std::string, std::string>::iterator it = this->_loginfo.find(username);

	if (it == this->_loginfo.end())
		return (false);
	if (passwd != it->second)
		return (false);
	return (true);

}

std::ostream&	operator<<(std::ostream &os, const location &loc)
{
	std::vector<std::string> vc;
	std::vector<std::string>::iterator it;

	os << std::setw(13) << std::left << "\tmatch: " << loc.getMatch() << std::endl;
	os << std::setw(13) << std::left << "\tautoindex: ";
	if (loc.getAutoindex())
		os << "on" << std::endl;
	else
		os << "off" << std::endl;
	os << std::setw(13) << std::left << "\troot: " << loc.getRoot() << std::endl;

	vc = loc.getMethods();
	it = vc.begin();

	os << std::setw(13) << std::left << "\tmethods: " << std::endl;
	while (!vc.empty() && it != vc.end())
	{
		os << "\t\t- " << *it << std::endl;
		it++;
	}

	os << std::setw(13) << std::left << "\terror page: " << loc.getErrorPage() << std::endl;

	vc = loc.getIndices();
	it = vc.begin();

	os << std::setw(13) << std::left << "\tindices: " << std::endl;
	while (!vc.empty() && it != vc.end())
	{
		os << "\t\t- " << *it << std::endl;
		it++;
	}

	os << std::setw(13) << std::left << "\tauthBasic: " << loc.getAuthBasic() << std::endl;
	os << std::setw(13) << std::left << "\tauthFile: " << loc.getAuthUserFile() << std::endl;
	os << std::setw(13) << std::left << "\tcgi path: " << loc.getCgiPath() << std::endl;
	return (os);
}
