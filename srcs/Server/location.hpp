#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <string>
# include <vector>
# include <map>

class location {
public:
	typedef void	(location::*setter)(std::string&);

	class	inputErrorException : public std::exception {
	public:
		virtual const char*	what() const throw();
	};

private:
	bool							_autoindex;
	std::string						_match;
	std::string						_root;
	std::vector<std::string>		_methods;
	std::string						_errorPage;
	std::vector<std::string>		_indices;
	std::map<std::string, setter>	_typeFunctionMap;

	std::string						_authBasic;
	std::string						_authBasicUserFile;
	std::string						_cgiPath;

	bool							_isFileExtension;
	location();

public:
	explicit location(std::string &match);
	location(location const &original);
	~location();
	location& operator=(location const &original);

	void	setAutoindex(std::string &autoindex);
	void	setRoot(std::string &root);
	void	setMethod(std::string &method);
	void	setErrorPage(std::string &errorPage);
	void	setIndices(std::string &indices);
	void	setCgiPath(std::string &cgiPass);
	void	setAuthBasic(std::string &authBasic);
	void	setAuthUserFile(std::string &userFile);

	const bool						&getAutoindex() const;
	const std::string				&getMatch() const;
	const std::string				&getRoot() const;
	const std::vector<std::string>	&getMethods() const;
	const std::string				&getErrorPage() const;
	const std::vector<std::string>	&getIndices() const;
	const std::string				&getCgiPath() const;
	const std::string				&getAuthBasic() const;
	const std::string				&getAuthUserFile() const;

	void	findValue(std::string &key, std::string line);
	bool	valueCheck() const;

	bool	isFileExtension() const;

};

std::ostream&	operator<<(std::ostream &os, const location &loc);

#endif
