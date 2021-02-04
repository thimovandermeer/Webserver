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
		FD_SET((*it).getSocketFd(), &this->readFds);
		this->_nrOfServers++;
		it++;
	}
}

void	serverCluster::startListening()
{
	while (true)
	{
		fd_set			workingSet;
		int 			ret;

		memcpy(&workingSet, &this->readFds, sizeof (this->readFds));
		FD_SET(0, &workingSet); // adding stdin to readfds

		std::cout << "waiting for connection.." << std::endl;
		ret = select(this->_nrOfServers * NR_OF_CONNECTIONS + 1, &workingSet, NULL, NULL, NULL);
		if (FD_ISSET(0, &workingSet)) //input from stdin
		{
			std::string	line;
			std::getline(std::cin, line);
			if (line == "exit")
			{
				std::cout << "exiting..." << std::endl;
				exit (0);
			}
			else
				std::cout << "unknown input" << std::endl;
		}
		if (ret > 0)
			std::cout << "Houston we have contact" << std::endl;
		std::vector<server>::iterator it = this->_servers->begin();
		while (!this->_servers->empty() && it != this->_servers->end() && ret)
		{
			long fd  = it->getSocketFd();
			if (workingSet.fds_bits[fd / 64] & (long)(1UL << fd % 64)) {
				it->run();
				ret--;
			}
			it++;
		}
	}
}
