#include "serverCluster.hpp"
#include <sys/select.h>
#include "../Request/request.hpp"
#include "../Utils/utils.hpp"
#include <string.h>

serverCluster::serverCluster() : _nrOfServers(0), _highestFd(0)
{
	FD_ZERO(&this->readFds);
	FD_ZERO(&this->writeFds);
}

serverCluster::serverCluster(const serverCluster &original)
{
	*this = original;
}

serverCluster::~serverCluster()
{
	std::vector<server*>::iterator it;
	it = this->_servers.begin();
	while (!this->_servers.empty() && it != this->_servers.end())
	{
		delete (*it);
		it++;
	}
}

serverCluster	&serverCluster::operator=(const serverCluster &original)
{
	this->_servers = original._servers;
	this->_nrOfServers = original._nrOfServers;
	return (*this);
}

void	serverCluster::addServer(server *newServ)
{
	this->_servers.push_back(newServ);
}

bool	serverCluster::isEmpty() const
{
	return (this->_servers.empty());
}

void	serverCluster::startup()
{
	std::vector<server*>::iterator it = this->_servers.begin();
	while (!this->_servers.empty() && it != this->_servers.end())
	{
		(*it)->startListening();
		FD_SET((*it)->getSocketFd(), &this->readFds);
		this->_highestFd = std::max(this->_highestFd, (*it)->getSocketFd());
		this->_nrOfServers++;
		it++;
	}
}

void	serverCluster::startListening()
{
	while (true)
	{
		fd_set			readSet;
		fd_set			writeSet;
		int 			ret;
		long			maxFd = this->_highestFd;
		std::vector<server*>::iterator it = this->_servers.begin();

		FD_ZERO(&writeSet);
		FD_ZERO(&readSet);
		memcpy(&readSet, &this->readFds, sizeof (this->readFds));
		while (it != this->_servers.end())
		{
			for (int i = 0; i < NR_OF_CONNECTIONS; i++)
			{
				if ((*it)->connections[i].getAcceptFd() != -1)
				{
					if ((*it)->connections[i].doINeedToFuckingCloseThisShitIDFK())
						continue;
					unsigned long a = getTime();
					unsigned long b = (*it)->connections[i].getTimeLastRead();
					if (CONNECTION_TIMEOUT > 0 && a - b > CONNECTION_TIMEOUT && (*it)->connections[i].getResponseString().empty())
					{
						std::cerr << "closing connection" << std::endl;
						if (!(*it)->connections[i].getBuffer().empty())
						{
							(*it)->generateResponse(i);
							(*it)->connections[i].sendData((*it)->_bodylen);
						}
						(*it)->connections[i].resetConnection();
						(*it)->connections[i].closeConnection();
						continue;
					}
					maxFd = std::max(maxFd, (*it)->connections[i].getAcceptFd());
					if (!(*it)->connections[i].hasFullRequest())
						FD_SET((*it)->connections[i].getAcceptFd(), &readSet);
					else
						FD_SET((*it)->connections[i].getAcceptFd(), &writeSet);
				}
			}
			it++;
		}
//		std::cout << "waiting for connection.." << std::endl;
		struct timeval timeout;
		timeout.tv_sec = SELECT_TIMEOUT;
		timeout.tv_usec = 0;
		ret = select(maxFd + 1, &readSet, &writeSet, NULL, &timeout);
		it = this->_servers.begin();
		while (it != this->_servers.end() && ret) // gebeurt per server
		{
//			std::cout << "Still looping" << std::endl;
			long fd;
			fd = (*it)->getSocketFd(); // check of nieuwe verbinding op socket
			if (readSet.fds_bits[fd / 64] & (long)(1UL << fd % 64))
			{
				if ((*it)->acpt() == 1)
					break;
			}
			for (int i = 0; i < NR_OF_CONNECTIONS; i++)
			{
				if ((*it)->connections[i].getAcceptFd() != -1) // er moet van gelezen of naar geschreven worden
				{
					fd = (*it)->connections[i].getAcceptFd();
					if (readSet.fds_bits[fd / 64] & (long)(1UL << fd % 64))
					{
						(*it)->connections[i].startReading(); // start reading
						break;
					}
					if (writeSet.fds_bits[fd / 64] & (long)(1UL << fd % 64))
					{
						(*it)->generateResponse(i);
						(*it)->connections[i].sendData((*it)->_bodylen); // start writing
						break;
					}
				}
			}
			it++;
		}
	}
}
