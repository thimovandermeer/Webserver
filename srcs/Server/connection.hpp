#ifndef WEBSERV_CONNECTION_HPP
# define WEBSERV_CONNECTION_HPP
# include <string>
# define BUFFSIZE 4095

class connection {
private:


public:

	unsigned long	timeLastRead;
	long			acceptFd;
	std::string		acceptBuffer;
	bool			hasFullRequest;
	bool	isFullRequest(std::string &currentRequest) const;


	connection();
	~connection();
	connection(const connection &original);
	connection& operator=(const connection &original);

	void	closeThisConnection();
	void	sendData(std::string &response);
	std::string	receive() const;
	void	startReading();
};

#endif
