#include "responseHeader.hpp"

responseHeader::responseHeader(std::string &content, std::string &path, int status, std::string &contentType)
{
	setAllow(status);
	setContentLanguage();
	setContentLength(content.length());
	setContentLocation(path, status);
	setContentType(contentType);
	setDate();
	setLastModified(path);
	setLocation(path, status);
	setRetryAfter(status, 10);
	setServer();
	setTransferEncoding();
	setWwwAuthenticate(status);
}


responseHeader::responseHeader(const responseHeader &src)
{
	_acceptCharsets = src._acceptCharsets;
	_acceptLanguage =src._acceptLanguage;
	_allow = src._allow;
	_authorization = src._authorization;
	_contentLanguage =src._contentLanguage;
	_contentLength = src._contentLength;
	_contentType = src._contentType;
	_contentLocation = src._contentLocation;
	_date = src._date;
	_host = src._host;
	_lastModified = src._lastModified;
	_location = src._location;
	_referer = src._referer;
	_retryAfter = src._retryAfter;
	_server = src._server;
	_transferEncoding = src._transferEncoding;
	_wwwAuthenticate = src._wwwAuthenticate;
}

 responseHeader::~responseHeader()
{

}

responseHeader &responseHeader::operator=(const responseHeader &src)
{
	_acceptCharsets = src._acceptCharsets;
	_acceptLanguage = src._acceptLanguage;
	_allow = src._allow;
	_authorization = src._authorization;
	_contentLanguage =src._contentLanguage;
	_contentLength = src._contentLength;
	_contentType = src._contentType;
    _contentLocation = src._contentLocation;
    _date = src._date;
	_host = src._host;
	_lastModified = src._lastModified;
	_location = src._location;
	_referer = src._referer;
	_retryAfter = src._retryAfter;
	_server = src._server;
	_transferEncoding = src._transferEncoding;
	_wwwAuthenticate = src._wwwAuthenticate;
	return (*this);
}

std::string responseHeader::getHeader(int status)
{
	std::string header;
	std::stringstream ss;
	ss << status;

	header = "HTTP/1.1 " + ss.str() + " " + createStatusMessage(status) + "\r\n";
	header += writeHeader();
	return (header);
}

std::string		responseHeader::createStatusMessage(int status)
{
	if(status == 200)
		return ("OK");
	else if (status == 201)
		return ("Created");
	else if (status == 204)
		return ("No Content");
	else if (status == 400)
	        return ("Bad Request Error");
	else if (status == 403)
		return ("Forbidden");
	else if (status == 404)
		return ("Not _found");
	else if (status == 405)         
	    return ("Method Not Allowed");
	else if (status == 401)
		return ("Unauthorized");
	else if (status == 413)
		return ("Payload too large");
	else if (status == 503)
		return ("Service unavailable");
	else
		return ("Unknown error");
}

#include <cstdlib>
std::string 		responseHeader::writeHeader()
{
	std::string header;

	if (!_allow.empty())
		header += "Allow: " + _allow + "\r\n";
	if (!_contentLanguage.empty())
		header += "Content-Language: " + _contentLanguage + "\r\n";

	if (strtol(this->_contentLength.c_str(), NULL, 10) < MAXSENDSIZE)
		header += "Content-Length: " + _contentLength + "\r\n";
	else
		header += "Transfer-Encoding: " + _transferEncoding + "\r\n";
	if (!_contentLocation.empty())
		header += "Content-Location: " + _contentLocation + "\r\n";
	if (!_contentType.empty())
		header += "Content-Type: " + _contentType + "\r\n";
	if (!_date.empty())
		header += "Date: " + _date + "\r\n";
	if (!_lastModified.empty())
		header += "Last-Modified: " + _lastModified + "\r\n";
	if (!_location.empty())
		header += "Location: " + _location + "\r\n";
	if (!_retryAfter.empty())
		header += "Retry-After: " + _retryAfter + "\r\n";
	if (!_server.empty())
		header += "Server: " + _server + "\r\n";
	if (!_wwwAuthenticate.empty())
		header += "Www-Authenticate: " + _wwwAuthenticate + "\r\n";
	if (!_specialHeaderKey.empty() && !_specialHeaderValue.empty())
	    header += _specialHeaderKey + ": " + _specialHeaderValue + "\r\n";
	header += "\r\n";
	return (header);
}

void responseHeader::setContentLocation(const std::string &path, int status)
{
	if (status != 404)
		_contentLocation = path;
}

void responseHeader::setAllow(const int &status)
{
	if (status == 405)
		_allow = "Get, Head, Post, Put";
	else
		_allow = "";
}

void responseHeader::setContentLanguage()
{
	_contentLanguage = "en-US";
}

void responseHeader::setContentLength(int length)
{
	std::stringstream ss;
	ss << length;
	_contentLength = ss.str();
}

void responseHeader::setContentType(const std::string &contentType)
{
	_contentType = contentType;
}

void responseHeader::setDate()
{
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_date = buf; 
}

void responseHeader::setLastModified(const std::string &path)
{
	struct stat stats;
	struct tm *tm;
	char buf[1000];

	if (stat(path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buf,sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", tm);
		_lastModified = buf;
	}
}

void responseHeader::setLocation(const std::string &path, int status)
{
	if (status == 201 || status / 100 == 3)
		_location = path;
}

void responseHeader::setServer()
{
	_server = "Epic Merel Jonas Thimo HTTP Server HUTS";
}

void responseHeader::setTransferEncoding()
{
	_transferEncoding = "Chunked";
}

void responseHeader::setWwwAuthenticate(int status)
{
	if (status == 401)
		_wwwAuthenticate = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
}

void responseHeader::setRetryAfter(int status, int number)
{
	if (status == 503)
	{
		char buf[1000];
		time_t now = time(0);
		struct tm tm = *gmtime(&now);
		tm.tm_hour += 1;
		strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
		_retryAfter = buf; 
	}
	else if (status / 100 == 3) {
		std::stringstream ss;
		ss << number;
		_retryAfter = ss.str();
	}
	else
		_retryAfter = "";
}
