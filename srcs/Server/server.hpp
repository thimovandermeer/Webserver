#ifndef SERVER_HPP
# define SERVER_HPP
# include "location.hpp"
# include "connection.hpp"
# include "../Utils/defines.hpp"
# include <vector>
# include <map>
# include <stdexcept>
# include <string>
# include <sys/socket.h>
# include <netdb.h>
# include <sstream>
# include <iostream>
# include <iomanip>
# include <unistd.h>
# include <climits>

class server {
public:
	typedef void	(server::*setter)(std::string&);

	class	inputErrorException : public std::exception {
	public:
		virtual const char*	what() const throw();
	};

	class	syscallErrorException : public std::exception {
	public:
		virtual const char*	what() const throw();
	};

private:
	int								_portNr;
	size_t							_maxBodySize;
	bool							_autoindex;
	std::string						_root;
	std::string						_errorPage;
	std::string						_host;
	std::vector<std::string>		_serverNames;
	std::vector<std::string>		_indices;
	std::map<std::string, setter>	_typeFunctionMap;
	std::vector<location*>			_locations;
	std::vector<server*>            _alternativeServers;

	long				_socketFd;
	struct sockaddr_in	_addr;

public:
	size_t				_bodylen;
	connection			connections[NR_OF_CONNECTIONS];

	server();
	server(server const &original);
	~server();
	server&	operator=(server const &original);

	void	setPort(std::string &listen);
	void	setMaxBodySize(std::string &size);
	void	setAutoindex(std::string &autoindex);
	void	setRoot(std::string &root);
	void	setErrorPage(std::string &page);
	void	setHost(std::string &host);
	void	setServerNames(std::string &names);
	void	setIndices(std::string &indices);
	void    setAlternativeServers(server *alternative);

	void	setupRespStr(int index);

	const int						&getPortNr() const;
	const size_t					&getMaxBodySize() const;
	const bool						&getAutoindex() const;
	const std::string				&getRoot() const;
	const std::string				&getErrorPage() const;
	const std::string				&getHost() const;
	const std::vector<std::string>	&getServerNames() const;
	const std::vector<std::string>	&getIndices() const;
	const std::vector<location*>	&getLocations() const;

	const long						&getSocketFd() const;
	const struct sockaddr_in		&getAddr() const;

	void		addLocation(location *newLoc);
	bool		valueCheck() const;
	int 		acpt();
	void		findValue(std::string &key, std::string line);
	void	    startListening();
	location*	findLocation(std::string &match);
	void        createResponse(int index);
};

std::ostream&	operator<<(std::ostream &os, const server &serv);

#endif
