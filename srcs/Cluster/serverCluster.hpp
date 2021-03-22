#ifndef SERVERCLUSTER_HPP
# define SERVERCLUSTER_HPP
# include "../Server/server.hpp"
# include "../Utils/defines.hpp"

class serverCluster {
public:
	fd_set	readFds;
	fd_set	writeFds;

private:
	std::vector<server*>	_servers;
	int						_nrOfServers;

public:
	serverCluster();
	serverCluster(const serverCluster &original);
	~serverCluster();
	serverCluster&	operator=(const serverCluster &original);

	void	addServer(server *newServ);
	bool	isEmpty() const;

	void	startup();

	void	startListening();
};

#endif
