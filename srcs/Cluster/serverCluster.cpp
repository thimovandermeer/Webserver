#include "serverCluster.hpp"
#include <sys/select.h>
#include "../Request/request.hpp"

serverCluster::serverCluster() : _nrOfServers(0)
{
	this->_servers = new std::vector<server>;
	FD_ZERO(&this->readFds);
	FD_ZERO(&this->writeFds);
}

serverCluster::serverCluster(const serverCluster &original)
{
	*this = original;
}

serverCluster::~serverCluster()
{
	delete(this->_servers);
}

serverCluster	&serverCluster::operator=(const serverCluster &original)
{
	this->_servers = original._servers;
	return (*this);
}

void	serverCluster::addServer(server &newServ)
{
	this->_servers->push_back(newServ);
}

bool	serverCluster::isEmpty() const
{
	return (this->_servers->empty());
}

void	serverCluster::startup()
{
	std::vector<server>::iterator it = this->_servers->begin();
	while (!this->_servers->empty() && it != this->_servers->end())
	{
		(*it).startListening();
		FD_SET((*it).getListenFd(), &this->readFds);
		this->_nrOfServers++;
		it++;
	}
}

void	serverCluster::startListening()
{
	int n = 1; // this is just to get rid of clang-tidy for now
	while (n > 0)
	{
		struct timeval	timeout;
		fd_set			workingSet;
		int 			ret;
		memcpy(&workingSet, &this->readFds, sizeof (this->readFds));
		timeout.tv_sec = 5;
		timeout.tv_usec = 0; // timeout of 1 sec

		std::cout << "waiting for connection" << std::endl;
		ret = select(this->_nrOfServers * NR_OF_CONNECTIONS + 1, &workingSet, NULL, NULL, &timeout);
		if (ret > 0)
			std::cout << "Houston we have contact" << std::endl;
		std::vector<server>::iterator it = this->_servers->begin();
		while (!this->_servers->empty() && it != this->_servers->end() && ret)
		{
			long fd  = it->getListenFd();
			if (workingSet.fds_bits[fd / 64] & (long)(1UL << fd % 64)) {
				it->run();
				ret--;
			}
			it++;
		}
	}
}
