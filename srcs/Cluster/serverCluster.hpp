#ifndef SERVERCLUSTER_HPP
# define SERVERCLUSTER_HPP
# include "../Server/server.hpp"
# include "../Utils/defines.hpp"
# include "../webserv.hpp"
# include <algorithm>

class serverCluster {
public:
	fd_set	readFds;
	fd_set	writeFds;

private:
	std::vector<server*>	_servers;
	int						_nrOfServers;
	long					_highestFd;

public:
	typedef void	(server::*setter)(std::string&);

	class	duplicatePortException : public std::exception {
	public:
		virtual const char*	what() const throw();
	};
	serverCluster();
	serverCluster(const serverCluster &original);
	~serverCluster();
	serverCluster&	operator=(const serverCluster &original);

	void	addServer(server *newServ);
	bool	isEmpty() const;

	void	duplicatePorts() const;
	void	startup();

	void	startListening();
};

#endif
