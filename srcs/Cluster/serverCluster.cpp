#include "serverCluster.hpp"
#include <sys/select.h>
#include "../Request/request.hpp"
#include "../Utils/utils.hpp"
#include "../Response/Response.hpp"
#include <string.h>

connection *g_recentConnection;


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

// in de server
//std::vector<servers*>       *Allservers;
//(*it)->servers = &this->servers

// dit moet ik helemaal gaan herschrijven
// ik moet door alle servers loopen en tegelijk
// checken of een port dubbel is
// als een port dubbel is, moet ik de pointer naar die server
// opslaan in de initiele server
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
	        if((*it1)->getPortNr() == (*it2)->getPortNr()){
                (*it1)->_alternativeServers.push_back(*it2);
                std::cout << (*it1)->getPortNr() << " " <<  (*it2)->getPortNr() << std::endl ;
	        }
	        it2++;
	    }
    }
//	std::map<int, int>::iterator it1;
//	std::map<int, int>::iterator it2;
//	for (it1 = ports.begin(); it1 != ports.end(); it1++) {
//        it2 = it1;
//        it2++;
//        while (it2 != ports.end()) {
//            if (it1->second == it2->second) {
//                this->_boolDoublePorts = true;
//                this->_doublePorts.insert(std::make_pair(it1->second, std::map<int, int>()));
//                this->_doublePorts[it1->second].insert(std::make_pair(it1->first, it2->first));
//                break;
//            }
//            it2++;
//        }
//    }
//    std::map<int, std::map<int, int> >::iterator itr;
//	std::map<int, int>::iterator ptr;
//	for(itr = _doublePorts.begin(); itr != _doublePorts.end(); itr++) {
//	    for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
//	        std::cout << itr->first << " " << ptr->first << " " << ptr->second << std::endl ;
//	    }
//	}
}

//std::vector<server*>::const_iterator    it;
//std::map<int, int>                      ports;
//
//if (this->_servers.size() < 2)
//return;
//int i = 0;
//for (it = this->_servers.begin(); it != this->_servers.end(); it++) {
//ports.insert(std::make_pair(i, (*it)->getPortNr()));
//i++;
//}
//std::map<int, int>::iterator it1;
//std::map<int, int>::iterator it2;
//for (it1 = ports.begin(); it1 != ports.end(); it1++) {
//it2 = it1;
//it2++;
//while (it2 != ports.end()) {
//if (it1->second == it2->second) {
//this->_boolDoublePorts = true;
//this->_doublePorts.insert(std::make_pair(it1->second, std::map<int, int>()));
//this->_doublePorts[it1->second].insert(std::make_pair(it1->first, it2->first));
//break;
//}
//it2++;
//}
//}
//std::map<int, std::map<int, int> >::iterator itr;
//std::map<int, int>::iterator ptr;
//for(itr = _doublePorts.begin(); itr != _doublePorts.end(); itr++) {
//for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
//std::cout << itr->first << " " << ptr->first << " " << ptr->second << std::endl ;
//}
//}

std::map< int, std::map <int, int> > serverCluster::getDoublePorts() const {
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
        //_servers heeft de servers waaruit we kunnen kiezen
		g_recentConnection = NULL;
		signal(SIGPIPE, sigPipeHandler);
		FD_ZERO(&writeSet);
		FD_ZERO(&readSet);
		readSet = this->readFds;
		while (it != this->_servers.end())
		{   //merel -> waar wordt hier nou precies de keuze gemaakt voor welke server we kiezen
		    //merel -> en hoe kan ik de host uit respons nog ophalen?
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
		{       // -> merel hier wordt er gehopt tussen de servers en gecheckt welke werkt
		        // dus ik zou zeggen dat als er een dubbele port is dat je hier bepaalt welke de juiste
		        // server is om te pakken
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

/*Pseudo code voor de te kiezen server
 * er is een vector van _servers en ik weet de locaties
 * van de ports die hetzelfde zijn. de locaties  van waar
 * deze dubbele locaties staan zijn opgeslgen in _doublePorts,
 * een map<int map< int, int> >, van die locaties doe ik de get->serverName
 * en die vergelijk ik met de host van request. De locatie van
 * de host en servername die overeenkomen is hoe ver je door
 * de _servers moet iteraten om bij de juiste server te komen.
 * Als geen een van de servers overeenkomen, pakken we de
 * laagste locatie
 * - waar wordt de juiste port bepaald?
 * - waar wordt de keuze precies gemaakt voor de server?
 * Dit is de plek waar we moeten ingrijpen */