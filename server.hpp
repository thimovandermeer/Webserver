#ifndef SERVER_HPP
# define SERVER_HPP
# include "webserv.hpp"
# include <vector>

class server {
private:
	int							_portNr;
	size_t						_maxBodySize;
	bool						_autoindex;
	std::string					_root;
	std::string					_errorPage;
	std::string					_host;
	std::vector<std::string>	_serverNames;
	std::vector<std::string>	_indices;
	// vector filled with locations;
public:
	server();
	server(server const &original);
	~server();
	server&	operator=(server const &original);

	void	setPort(int portNr);
	void	setRoot(std::string &root);
	void	setServerNames(std::string &names);
	void	setErrorPage(std::string &page);
	void	setmaxBodySize(size_t size);
	void	setIndices(std::string &indices);
	void	setAutoindex(std::string &autoindex);
	void	setHost(std::string &host);

	int							getPortNr() const;
	size_t						getMaxBodySize() const;
	bool						getAutoindex() const;
	std::string					getRoot() const;
	std::string					getErrorPage() const;
	std::string					getHost() const;
	std::vector<std::string>	getServerNames() const;
	std::vector<std::string>	getIndices() const;

	bool	valueCheck() const;
};


#endif //WEBSERV_SERVER_HPP
