//
// Created by Thimo Van der meer on 25/01/2021.
//

#include "ResponseHeader.hpp"
#include "Response.hpp"
#include <sys/time.h>
#include <sys/stat.h>
// canonical form functions
ResponseHeader::ResponseHeader(std::string &content, std::string &path, int code, std::string &contentType)
{
	setAllow(code);
	setContentLanguage();
	setContentLength(content.length());
	setContentLocation(path, code);
	setContentType(contentType);
	setDate();
	setLastModified(path);
	setLocation(path, code);
	setRetryAfter(code, 10);
	setServer();
//	setTransferEncoding();
	setWwwAuthenticate(code);
}

ResponseHeader::ResponseHeader(const ResponseHeader &src)
{
	_acceptCharsets = src._acceptCharsets;
	_acceptLanguage =src._acceptLanguage;
	_allow = src._allow;
	_authorization = src._authorization;
	_contentLanguage =src._contentLanguage;
	_contentLength = src._contentLength;
	_contentType = src._contentType;
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

ResponseHeader::~ResponseHeader()
{

}

ResponseHeader &ResponseHeader::operator=(const ResponseHeader &src)
{
	_acceptCharsets = src._acceptCharsets;
	_acceptLanguage = src._acceptLanguage;
	_allow = src._allow;
	_authorization = src._authorization;
	_contentLanguage =src._contentLanguage;
	_contentLength = src._contentLength;
	_contentType = src._contentType;
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

// public member functions
std::string ResponseHeader::getHeader(int code)
{
	std::string header;
	// set all headers to appropriate info
//	setAllHeaders(content, path, code, contentType);
	// write header
	header = "HTTP/1.1 " + std::to_string(code) + " " + createStatusMessage(code) + "\r\n";
	header += writeHeader();
	return (header);
}

std::string		ResponseHeader::createStatusMessage(int code)
{
	// trying to solve this with an enum or some other smart data type
	if(code == 200)
		return ("OK");
	else if (code == 201)
		return ("Created");
	else if (code == 204)
		return ("No Content");
	else if (code == 403)
		return ("Forbidden");
	else if (code == 404)
		return ("Not found");
	else
		return ("Zieke Error in onze code");
}

std::string 		ResponseHeader::writeHeader()
{
	std::string header;

	if (_allow != "")
		header += "Allow: " + _allow + "\r\n";
	if (_contentLanguage != "")
		header += "Content-Language: " + _contentLanguage + "\r\n";
	if (_contentLength != "")
		header += "Content-Length: " + _contentLength + "\r\n";
	if (_contentLocation != "")
		header += "Content-Location: " + _contentLocation + "\r\n";
	if (_contentType != "")
		header += "Content-Type: " + _contentType + "\r\n";
	if (_date != "")
		header += "Date: " + _date + "\r\n";
	if (_lastModified != "")
		header += "Last-Modified: " + _lastModified + "\r\n";
	if (_location != "")
		header += "Location: " + _location + "\r\n";
	if (_retryAfter != "")
		header += "Retry-After: " + _retryAfter + "\r\n";
	if (_server != "")
		header += "Server: " + _server + "\r\n";
	if (_transferEncoding != "")
		header += "Transfer-Encoding: " + _transferEncoding + "\r\n";
	if (_wwwAuthenticate != "")
		header += "Www-Authenticate: " + _wwwAuthenticate + "\r\n";
	header += "\r\n";
	return (header);
}


// setters
void ResponseHeader::setContentLocation(const std::string &path, int code)
{
	if (code != 404)
		_contentLocation = path;
}

void ResponseHeader::setAllow(const int &code)
{
	if (code == 405)
	{
		_allow = "Get, Head, Post, Put";
	}
	else
		_allow = "";
}

void ResponseHeader::setContentLanguage()
{
	_contentLanguage = "en-US";
}

void ResponseHeader::setContentLength(int length)
{
	_contentLength = std::to_string(length);
}

void ResponseHeader::setContentType(const std::string &contentType)
{
	_contentType = contentType;
}

void ResponseHeader::setDate()
{
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_date = buf; // might cast in to a string lets check later
}

void ResponseHeader::setLastModified(const std::string &path)
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

void ResponseHeader::setLocation(const std::string &path, int code)
{
	if (code == 201 || code / 100 == 3)
	{
		_location = path;
	}
}

void ResponseHeader::setServer()
{
	_server = "Epic Merel Jonas Thimo HTTP Server HUTS";
}

void ResponseHeader::setTransferEncoding()
{
	_transferEncoding = "Chunked";
}

void ResponseHeader::setWwwAuthenticate(int code)
{
	if (code == 401)
	{
		_wwwAuthenticate = "Newauth realm=\"apps\", type=1,\n title=\"Login to \\\"apps\\\"\", Basic realm=\"simple\"charset=\"UTF-8\"";
	}
}

// create proper testers
void ResponseHeader::setRetryAfter(int code, int number)
{
	if (code == 503)
	{
		char buf[1000];
		time_t now = time(0);
		struct tm tm = *gmtime(&now);
		tm.tm_hour += 1;
		strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
		_retryAfter = buf; // might need to convert it to a string lets see later
	}
	else if (code / 100 == 3)
	{
		_retryAfter = std::to_string(number);
	}
	else
		_retryAfter = "";
}

// Getters for testing

const std::string &ResponseHeader::getAcceptCharsets() const
{
	return _acceptCharsets;
}

const std::string &ResponseHeader::getAcceptLanguage() const
{
	return _acceptLanguage;
}

const std::string &ResponseHeader::getAllow() const
{
	return _allow;
}

const std::string &ResponseHeader::getAuthorization() const
{
	return _authorization;
}

const std::string &ResponseHeader::getContentLanguage() const
{
	return _contentLanguage;
}

const std::string &ResponseHeader::getContentLocation() const
{
	return _contentLocation;
}

const std::string &ResponseHeader::getContentLength() const
{
	return _contentLength;
}

const std::string &ResponseHeader::getContentType() const
{
	return _contentType;
}

const std::string &ResponseHeader::getDate() const
{
	return _date;
}

const std::string &ResponseHeader::getHost() const
{
	return _host;
}

const std::string &ResponseHeader::getLastModified() const
{
	return _lastModified;
}

const std::string &ResponseHeader::getLocation() const
{
	return _location;
}

const std::string &ResponseHeader::getReferer() const
{
	return _referer;
}

const std::string &ResponseHeader::getRetryAfter() const
{
	return _retryAfter;
}

const std::string &ResponseHeader::getServer() const
{
	return _server;
}

const std::string &ResponseHeader::getTransferEncoding() const
{
	return _transferEncoding;
}

const std::string &ResponseHeader::getWwwAuthenticate() const
{
	return _wwwAuthenticate;
}
