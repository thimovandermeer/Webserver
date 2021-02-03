#include "serverCluster.hpp"
#include <sys/select.h>
#include <iostream>

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
	struct timeval	timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0; // timeout of 1 sec

	int n = 1; // this is just to get rid of clang-tidy for now
	while (n > 0)
	{
		std::cout << "waiting for connection" << std::endl;
		select(this->_nrOfServers * NR_OF_CONNECTIONS + 1, &this->readFds, NULL, NULL, &timeout);
		std::vector<server>::iterator it = this->_servers->begin();
		while (!this->_servers->empty() && it != this->_servers->end())
		{
			server s = *it;
			if (FD_ISSET(s.getListenFd(), &this->readFds))
			{
				s.run();
				FD_SET(s.getConnectFd(), &this->writeFds);
			}

//			if (FD_ISSET(s.getConnectFd(), &this->writeFds))
//			{
//				// write response
//			}

			it++;
		}
	}
}
