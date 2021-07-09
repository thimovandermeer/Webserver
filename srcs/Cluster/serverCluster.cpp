#include "serverCluster.hpp"
#include <sys/select.h>
#include "../Request/request.hpp"
#include "../Utils/utils.hpp"
#include "../Response/Response.hpp"
#include <string.h>

connection *g_recentConnection;

/*
*	This constructor sets the read fd set and the write fd set to zero.
*/

serverCluster::serverCluster() : _nrOfServers(0), _highestFd(0)
{
	FD_ZERO(&this->readFds);
	FD_ZERO(&this->writeFds);
}

/*
*	Copy constructor
*/

serverCluster::serverCluster(const serverCluster &original)
{
	*this = original;
}

/*
*	This destructor iterates over the entire array of servers and deletes them all
*/

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

/*
*	assigment operator sets the servers and the number of servers
*/

serverCluster	&serverCluster::operator=(const serverCluster &original)
{
	this->_servers = original._servers;
	this->_nrOfServers = original._nrOfServers;
	return (*this);
}

/*
*	This function adds a server to the array of servers
*/

void	serverCluster::addServer(server *newServ)
{
	this->_servers.push_back(newServ);
}

/*
*	This function checks if the server array is empty
*/

bool	serverCluster::isEmpty() const
{
	return (this->_servers.empty());
}

/*
*	This function checks if there are two or more servers using the same ports
*	If thats the case we set a different port for the server
*/

void	serverCluster::duplicatePorts()
{
	std::vector<server*>::const_iterator    it1;
    std::vector<server*>::const_iterator    it2;

	if (this->_servers.size() < 2)
		return;

	for (it1 = this->_servers.begin(); it1 != this->_servers.end(); it1++) {
	    it2 = it1;
	    it2++;
	    while(it2 != this->_servers.end()){
	        if((*it1)->getPortNr() == (*it2)->getPortNr())
                (*it1)->setAlternativeServers(*it2);
	        it2++;
	    }
    }
}

/*
*	This function activates all the servers and makes sure that the highest fd is being stored
*/

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


/*
*	This function contains the main loop
*	In this function all servers start listening to incoming calls
*	if there is a incoming call this one is directed to a open listening server
*	The last part of this function handels our responses. It makes sure that the response is being send
*	Over the assigned write fd set
*/

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
							(*it)->createResponse(i);
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
					{
						FD_SET((*it)->connections[i].getAcceptFd(), &writeSet);
						if (!(*it)->connections[i].myresp)
							(*it)->createResponse(i);
					}
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
						if ((*it)->connections[connectioncounter].getResponseString().empty())
							(*it)->setupRespStr(connectioncounter);
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
