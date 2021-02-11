#ifndef SERVER_HPP
# define SERVER_HPP
# include "location.hpp"
# include <vector>
# include <map>
# include <stdexcept>
# include <string>
# include <sys/socket.h>
# include <netdb.h>

# define NR_OF_CONNECTIONS 10

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

	long				_socketFd;
	struct sockaddr_in	_addr;
	long				_acceptFd;

public:
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

	void	setAcceptFd(int fd);

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
	const long						&getAcceptFd() const;

	void		addLocation(location *newLoc);
	bool		valueCheck() const;
	std::string	receive() const;
	void 		sendData(const std::string &response) const;
	void 		serverClose();
	void 		accept();
	void		findValue(std::string &key, std::string line);

	void	startListening();

	void	run();

	location*	findLocation(std::string &match);
};

std::ostream&	operator<<(std::ostream &os, const server &serv);

#endif
