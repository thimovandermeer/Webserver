#include "Server.hpp"

Server::Server()
{}

Server::Server(Server& original)
{
	*this = original;
}

Server::~Server()
{}

Server&	Server::operator=(Server &original)
{
	return (*this);
}
