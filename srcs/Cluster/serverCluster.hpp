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

    std::vector<server*>	            _servers;
    std::map<int, std::map<int, int> >  _doublePorts;
    int						            _nrOfServers;
    long					            _highestFd;

public:
	serverCluster();
	serverCluster(const serverCluster &original);
	~serverCluster();
	serverCluster&	operator=(const serverCluster &original);

	void	addServer(server *newServ);
	bool	isEmpty() const;

	void	duplicatePorts();
	void	startup();

	void	startListening();
};

#endif
