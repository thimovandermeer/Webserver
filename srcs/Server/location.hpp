#ifndef LOCATION_HPP
# define LOCATION_HPP
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
# include <climits>
#include "../Utils/utils.hpp"
#include "../Utils/Base64.hpp"

class location {
public:
	typedef void	(location::*setter)(const std::string&);

	class	inputErrorException : public std::exception {
	public:
		virtual const char*	what() const throw();
	};

private:
	location();
	bool							_autoindex;
	bool							_ownAutoindex;
	size_t							_maxBodySize;
	bool							_ownBodySize;
	bool							_isFileExtension;
	std::string						_match;
	std::string						_root;
	std::vector<std::string>		_methods;
	std::string						_errorPage;
	std::vector<std::string>		_indices;
	std::map<std::string, setter>	_typeFunctionMap;

	std::string							_authBasic;
	std::string							_authBasicUserFile;
	std::string							_cgiPath;
	std::string 						_htpasswd_path;
	std::map<std::string, std::string>	_loginfo;

public:
	explicit location(std::string &match);
	location(location const &original);
	~location();
	location& operator=(location const &original);

	void	setAutoindex(const std::string &autoindex);
	void	setMaxBody(const std::string &size);
	void	setRoot(const std::string &root);
	void	setMethod(const std::string &method);
	void	setErrorPage(const std::string &errorPage);
	void	setIndices(const std::string &indices);
	void	setCgiPath(const std::string &cgiPass);
	void	setAuthBasic(const std::string &authBasic);
	void 	sethtpasswdpath(const std::string &path);


	const bool						&hasOwnAutoindex() const;
	const bool 						&hasOwnBodySize() const;
	const bool						&getAutoindex() const;
	const size_t					&getMaxBodySize() const;
	const std::string				&getMatch() const;
	const std::string				&getRoot() const;
	const std::vector<std::string>	&getMethods() const;
	const std::string				&getErrorPage() const;
	const std::vector<std::string>	&getIndices() const;
	const std::string				&getCgiPath() const;
	const std::string				&getAuthBasic() const;
	const std::string				&getAuthUserFile() const;
	bool 							getAuthMatch(const std::string& username, const std::string& passwd);
	std::string						gethtpasswdpath() const;
	void	                        findValue(std::string &key, std::string line);
	bool	                        valueCheck() const;
	bool	                        isFileExtension() const;

};

std::ostream&	operator<<(std::ostream &os, const location &loc);

#endif
