#include "connection.hpp"

connection::connection() : _timeLastRead(0), _acceptFd(-1), _hasFullRequest(false), _bodyBytesSent(0), _headerSent(false)
{

}

connection::~connection()
{
	close(this->_acceptFd);
}

connection::connection(const connection &original)
{
	*this = original;
}

connection &connection::operator=(const connection &original)
{
	this->_timeLastRead = original._timeLastRead;
	this->_acceptFd = original._acceptFd;
	this->_acceptBuffer = original._acceptBuffer;
	this->_hasFullRequest = original._hasFullRequest;
	return (*this);
}

void	connection::setTimeLastRead(const unsigned long &time)
{
	this->_timeLastRead = time;
}

void	connection::setFd(const long &fd)
{
	this->_acceptFd = fd;
}

void	connection::setFullReq(const bool &full)
{
	this->_hasFullRequest = full;
}

void	connection::setResponseString(const std::string &resp)
{
	this->_responseString = resp;
}

const unsigned long&	connection::getTimeLastRead() const
{
	return (this->_timeLastRead);
}

const long&			connection::getAcceptFd() const
{
	return (this->_acceptFd);
}

const bool&			connection::hasFullRequest() const
{
	return (this->_hasFullRequest);
}

const std::string&	connection::getBuffer() const
{
	return (this->_acceptBuffer);
}

const std::string&	connection::getResponseString() const
{
	return (this->_responseString);
}

void	connection::closeThisConnection()
{
	close(this->_acceptFd);
	this->_acceptFd = -1;
	this->_acceptBuffer.clear();
	this->_hasFullRequest = false;
	this->_timeLastRead = 0;
	this->_bodyBytesSent = 0;
	this->_headerSent = false;
	this->_responseString.clear();
}

void connection::sendData(const size_t bodylen)
{
	size_t headerlen = this->_responseString.find("\r\n\r\n") + 4;

	if (bodylen < (size_t)MAXSENDSIZE)
	{
		if (send(this->_acceptFd, this->_responseString.c_str(), this->_responseString.length(), 0) == -1)
			std::cerr << "send error" << std::endl;
		this->closeThisConnection();
	}
	else // chunked response
	{
		sendChunked(bodylen, headerlen);
	}
}

void	connection::sendChunked(const size_t bodylen, const size_t headerlen)
{
	if (!this->_headerSent)
	{
		std::string headr = this->_responseString.substr(0, headerlen);
		send(_acceptFd, headr.c_str(), headr.length(), 0);
		this->_headerSent = true;
		return;
	}
	else if (bodylen - this->_bodyBytesSent > 0) // send non-empty chunks
	{
		std::stringstream ss;
		size_t	bytesToSend = std::min((bodylen - this->_bodyBytesSent), (size_t)MAXSENDSIZE);

		ss << std::hex << bytesToSend;
		std::string chunk(ss.str());
		chunk += "\r\n";
		chunk.append(this->_responseString, headerlen + this->_bodyBytesSent, bytesToSend);
		chunk += "\r\n";
		send(_acceptFd, chunk.c_str(), chunk.length(), 0);
		this->_bodyBytesSent += bytesToSend;
	}
	else
	{
		std::string end("0\r\n\r\n");
		send(_acceptFd, end.c_str(), 5, 0);
		closeThisConnection();
	}
}

std::string	connection::receive() const
{
	char 	buffer[MAXREADSIZE + 1];
	int 	ret;

	bzero(buffer, MAXREADSIZE + 1);
	ret = recv(this->_acceptFd, buffer, MAXREADSIZE, 0);
	if (ret == -1)
	{
		std::cerr << "recv error" << std::endl;
//		throw syscallErrorException();
	}
	return (buffer);
}

void	connection::startReading()
{
	std::string receivedRequest;
//	this->acpt();
	try
	{
		receivedRequest = receive();
		this->_timeLastRead = getTime();
		this->_acceptBuffer += receivedRequest;
	}
	catch (std::exception &e)
	{
		return;
	}

	if (isFullRequest(this->_acceptBuffer))
		this->_hasFullRequest = true;
	else
		return;
}

bool	connection::isFullRequest(std::string &currentRequest) const
{
	size_t pos;

	pos = currentRequest.find("\r\n\r\n");
	if (pos == std::string::npos)
		return (false);

	if (currentRequest.find("Transfer-Encoding: chunked\r\n") != std::string::npos)
	{
		if (currentRequest.find("0\r\n\r\n", pos + 4) == currentRequest.length() - 5)
			return (true);
		else
			return (false);
	}
	else
		return (true);
}
