#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <unistd.h>
# include <sys/socket.h>
# include <iostream>
# include <algorithm>
# include <sstream>
# include "../Utils/utils.hpp"
# include "../Utils/defines.hpp"

class connection {
private:
	unsigned long	_timeLastContact;
	long			_acceptFd;
	std::string		_acceptBuffer;
	bool			_hasFullRequest;
	size_t			_bodyBytesSent;
	bool			_headerSent;
	std::string		_responseString;
	bool
	isFullRequest() const;

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

	void		resetConnection();
	void		closeConnection();
	void		sendData(const size_t bodylen);
	std::string	receive();
	void		startReading();
	bool		doINeedToFuckingCloseThisShitIDFK();
	void		sighandler(int sig);

	void		sendChunked(const size_t bodylen, const size_t headerlen);
};

#endif
