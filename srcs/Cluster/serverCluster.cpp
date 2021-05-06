#include "serverCluster.hpp"
#include <sys/select.h>
#include "../Request/request.hpp"
#include "../Utils/utils.hpp"
#include "../Response/Response.hpp"
#include <string.h>

connection *g_recentConnection;

//const char	*serverCluster::duplicatePortException::what() const throw()
//{
//	return ("duplicate ports specificied over multiple server blocks");
//}

serverCluster::serverCluster() : _nrOfServers(0), _highestFd(0), _boolDoublePorts(false)
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

bool    serverCluster::doublePort() const
{
    return _boolDoublePorts;
}

void	serverCluster::duplicatePorts()
{
	std::vector<server*>::const_iterator    it;
	std::map<int, int>                      ports;

	if (this->_servers.size() < 2)
		return;
	int i = 0;
	for (it = this->_servers.begin(); it != this->_servers.end(); it++) {
	    ports.insert(std::make_pair(i, (*it)->getPortNr()));
	    i++;
    }

	std::map<int, int>::iterator it1;
	std::map<int, int>::iterator it2;
	for (it1 = ports.begin(); it1 != ports.end(); it1++) {
        it2 = it1;
        it2++;
        while (it2 != ports.end()) {
            if (it1->second == it2->second) {
                this->_boolDoublePorts = true;
                this->_doublePorts.insert(std::make_pair(it1->first, it2->first));
                break;
            }
            it2++;
        }
    }
    std::map<int, int>::iterator it3;
    for (it3 = this->_doublePorts.begin(); it3 != this->_doublePorts.end(); it3++){
        std::cout << it3->first << " " << it3->second << std::endl ;
    }
}

std::map<int, int> serverCluster::getDoublePorts() const {
    return (this->_doublePorts);
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

		g_recentConnection = NULL;
		signal(SIGPIPE, sigPipeHandler);
		FD_ZERO(&writeSet);
		FD_ZERO(&readSet);
		readSet = this->readFds;
		while (it != this->_servers.end())
		{
			for (int i = 0; i < NR_OF_CONNECTIONS; i++)
			{
				if ((*it)->connections[i].getAcceptFd() != -1)
				{
					unsigned long a = getTime();
					unsigned long b = (*it)->connections[i].getTimeLastRead();
					if (CONNECTION_TIMEOUT > 0 && a - b > CONNECTION_TIMEOUT)
					{
						if (!(*it)->connections[i].getBuffer().empty())
						{
							g_recentConnection = &((*it)->connections[i]);
							(*it)->handleResponse(i);
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
		struct timeval timeout;
		timeout.tv_sec = SELECT_TIMEOUT;
		timeout.tv_usec = 0;
		if ((ret = select(maxFd + 1, &readSet, &writeSet, NULL, &timeout)) == -1)
		    exit(1);
		it = this->_servers.begin();
		while (it != this->_servers.end() && ret) 
		{
			long fd;
			fd = (*it)->getSocketFd(); 
			if (FD_ISSET(fd, &readSet))
			{
				if ((*it)->acpt() == 1)
					break;
			}
			static int connectioncounter = 0;
			for (int i = 0; i < NR_OF_CONNECTIONS; i++)
			{
				if ((*it)->connections[connectioncounter].getAcceptFd() != -1) 
				{
					fd = (*it)->connections[connectioncounter].getAcceptFd();
					if (FD_ISSET(fd, &readSet))
					{
						g_recentConnection = &((*it)->connections[connectioncounter]);
						(*it)->connections[connectioncounter].startReading();
						break;
					}
					if (FD_ISSET(fd, &writeSet))
					{
						g_recentConnection = &((*it)->connections[connectioncounter]);
						(*it)->handleResponse(connectioncounter);
						(*it)->connections[connectioncounter].sendData((*it)->_bodylen);
						break;
					}
				}
				connectioncounter++;
				connectioncounter %= NR_OF_CONNECTIONS;
			}
			it++;
		}
	}
}
