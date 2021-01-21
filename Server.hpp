#ifndef SERVER_HPP
# define SERVER_HPP
# include "webserv.hpp"
# include <vector>

class Server {
private:
	int							_portNr;
	std::string					_root;
	std::vector<std::string>	_serverNames;
public:
	Server();
	Server(Server &original);
	~Server();
	Server&	operator=(Server &original);
};


#endif //WEBSERV_SERVER_HPP
