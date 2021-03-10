#ifndef WEBSERV_CONNECTION_HPP
# define WEBSERV_CONNECTION_HPP
# include <string>

# define KB 1024
# define MB (KB * 1024)
# define MAXSENDSIZE (1 * MB)
# define MAXREADSIZE (1 * MB)

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
	void sendData(std::string &response, const size_t bodylen);
	std::string	receive() const;
	void	startReading();
};

#endif
