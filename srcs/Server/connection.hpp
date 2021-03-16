#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "../Utils/utils.hpp"

# define KB 1024
# define MB (KB * 1024)
# define MAXSENDSIZE (1 * MB)
# define MAXREADSIZE (1 * MB)

class connection {
private:
	unsigned long	_timeLastRead;
	long			_acceptFd;
	std::string		_acceptBuffer;
	bool			_hasFullRequest;
	size_t			_bodyBytesSent;
	bool			_headerSent;
	std::string		_responseString;
	bool
	isFullRequest(std::string &currentRequest) const;

public:
	connection();
	~connection();
	connection(const connection &original);
	connection& operator=(const connection &original);

	void	setTimeLastRead(const unsigned long &time);
	void	setFd(const long &fd);
	void	setFullReq(const bool &full);
	void	setResponseString(const std::string &resp);

	const unsigned long&	getTimeLastRead() const;
	const long&				getAcceptFd() const;
	const bool&				hasFullRequest() const;
	const std::string&		getBuffer() const;
	const std::string&		getResponseString() const;

	void		closeThisConnection();
	void		sendData(const size_t bodylen);
	std::string	receive() const;
	void		startReading();

	void		sendChunked(const size_t bodylen, const size_t headerlen);
};

#endif
