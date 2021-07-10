#include "connection.hpp"
#include "../Response/Response.hpp"

/*
*	Constructor which sets all member variabels needed to create the connection
*/

connection::connection() : _timeLastContact(0),
						   _acceptFd(-1), _hasFullRequest(false),
						   _bodyBytesSent(0), _headerSent(false),
						   myresp(NULL)
{}


/*
*	The only thing this destructor has to do is close the acceptfd_set
*/

connection::~connection()
{
	close(this->_acceptFd);
}

/*
*	simple copy Constructor
*/


connection::connection(const connection &original)
{
	*this = original;
}

/*
*	Assignment operator
*/

connection &connection::operator=(const connection &original)
{
	this->_timeLastContact = original._timeLastContact;
	this->_acceptFd = original._acceptFd;
	this->_acceptBuffer = original._acceptBuffer;
	this->_hasFullRequest = original._hasFullRequest;
	this->_bodyBytesSent = original._bodyBytesSent;
	this->_headerSent = original._headerSent;
	this->_responseString = original._responseString;
	return (*this);
}

/*
*	This function sets the time the server has had the last connection
*/


void	connection::setTimeLastRead(const unsigned long &time)
{
	this->_timeLastContact = time;
}

/*
*	This function sets the accept fd
*/

void	connection::setFd(const long &fd)
{
	this->_acceptFd = fd;
}

/*
*	This function sets the responsestring which is later being used in the response
*/

void	connection::setResponseString(const std::string &resp)
{
	this->_responseString = resp;
}

/*
*	This function returns the time the server has had the last connection
*/

const unsigned long&	connection::getTimeLastRead() const
{
	return (this->_timeLastContact);
}

/*
*	This function returns the current accept fd set
*/

const long&			connection::getAcceptFd() const
{
	return (this->_acceptFd);
}

/*
*	This function returns if the entire request has been received
*/

const bool&			connection::hasFullRequest() const
{
	return (this->_hasFullRequest);
}

/*
*	This function returns the accept buffer from the current connection
*/

const std::string&	connection::getBuffer() const
{
	return (this->_acceptBuffer);
}

/*
*	This function returns the full response string
*/


const std::string&	connection::getResponseString() const
{
	return (this->_responseString);
}

/*
*	This function resets the connection, it makes sure that everything is cleared
*/

void	connection::resetConnection()
{
	this->_acceptBuffer.clear();
	this->_hasFullRequest = false;
	this->_bodyBytesSent = 0;
	this->_headerSent = false;
	this->_responseString.clear();
	if (this->myresp)
		delete (this->myresp);
	this->myresp = NULL;
}

/*
*	This function entirely closes the connection, it assures that the underlying server is
*	Ready to retrieve new connections
*/

void	connection::closeConnection()
{
	close(this->_acceptFd);
	this->_acceptFd = -1;
	this->_timeLastContact = 0;
}

/*
*	This function sends the data from the response to the client
*/


void	connection::sendData(const size_t bodylen)
{
	size_t headerlen = this->_responseString.find("\r\n\r\n") + 4;

	if (bodylen < (size_t)MAXSENDSIZE)
	{
		if (send(this->_acceptFd, this->_responseString.c_str(), this->_responseString.length(), 0) == -1)
		{
			return;
		}
		this->resetConnection();
	}
	else // chunked response
		this->sendChunked(bodylen, headerlen);
	this->_timeLastContact = getTime();
}

/*
*	This functions sends the response in chunkes if the client wants chunked responses
*/

void	connection::sendChunked(const size_t bodylen, const size_t headerlen)
{
	size_t ret = 0;
	if (!this->_headerSent)
	{
		std::string headr = this->_responseString.substr(0, headerlen);
		ret = send(_acceptFd, headr.c_str(), headr.length(), 0);
		if (ret != headerlen)
		{
//			std::cerr << "send error" << std::endl;
			// sigpipe here is being handled elsewhere
			return;
		}
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
		ret = send(_acceptFd, chunk.c_str(), chunk.length(), 0);
		if (ret != chunk.length())
		{
//			std::cerr << "send error" << std::endl;
			// sigpipe here is being handled elsewhere
			return;
		}
		this->_bodyBytesSent += bytesToSend;
	}
	else
	{
		std::string end("0\r\n\r\n");
		ret = send(_acceptFd, end.c_str(), 5, 0);
		if (ret != 5)
		{
//			std::cerr << "send error" << std::endl;
			// sigpipe here is being handled elsewhere
			return;
		}
		this->resetConnection();
	}
}

/*
*	Constructor which sets all member variabels needed to create the connection
*/


std::string	connection::receive()
{
	char 	buffer[MAXREADSIZE + 1];
	int 	ret;

	bzero(buffer, MAXREADSIZE + 1);
	ret = recv(this->_acceptFd, buffer, MAXREADSIZE, 0);
	if (ret == -1)
	{
		std::cerr << "recv error" << std::endl;
		this->resetConnection();
		this->closeConnection();
	}
	if (ret == 0)
	{
		this->resetConnection();
		this->closeConnection();
	}
	return (buffer);
}

/*
*	This function makes sure that the connection is ready to start reading
*/

void	connection::startReading()
{
	std::string receivedRequest;
	try
	{
		receivedRequest = this->receive();
		this->_timeLastContact = getTime();
		this->_acceptBuffer += receivedRequest;
	}
	catch (std::exception &e)
	{
		return;
	}
	if (this->isFullRequest())
		this->_hasFullRequest = true;
}

/*
*	This function checks if the full request is received via the connection
*/

bool connection::isFullRequest() const
{
	size_t pos;
	pos = this->_acceptBuffer.find("\r\n\r\n");
	if (pos == std::string::npos)
		return (false);
	if (this->_acceptBuffer.find("Transfer-Encoding: chunked\r\n") != std::string::npos)
	{
		if (this->_acceptBuffer.find("0\r\n\r\n", pos + 4) == this->_acceptBuffer.length() - 5)
			return (true);
		else
			return (false);
	}
	if (this->_acceptBuffer.find("Content-Length: ") == std::string::npos) // content len not specified, so there is no body
		return (true);
	if (this->_acceptBuffer.find("\r\n\r\n", pos + 4) == std::string::npos) // there is content len, so a body, but no end of body found
		return (false);
	return (true);
}
