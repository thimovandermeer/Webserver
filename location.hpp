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
	std::string						_method;
	std::string						_errorPage;
	std::vector<std::string>		_indices;
	std::map<std::string, setter>	_typeFunctionMap;

// still gotta do something with CGI, but don't know what or how
	location();
public:
	explicit location(std::string &match);
	location(location const &original);
	~location();
	location& operator=(location const &original);

	void	setAutoindex(std::string &autoindex);
//	void	setMatch(std::string &match);
	void	setRoot(std::string &root);
	void	setMethod(std::string &method);
	void	setErrorPage(std::string &errorPage);
	void	setIndices(std::string &indices);

	const bool						&getAutoindex() const;
	const std::string				&getMatch() const;
	const std::string				&getRoot() const;
	const std::string				&getMethod() const;
	const std::string				&getErrorPage() const;
	const std::vector<std::string>	&getIndices() const;

	void	findValue(std::string &key, std::string line);
	bool	valueCheck() const;

};

std::ostream&	operator<<(std::ostream &os, const location &loc);

#endif
