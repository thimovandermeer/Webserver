//
// Created by Thimo Van der meer on 25/01/2021.
//

#ifndef WEBSERV_RESPONSEHEADER_HPP
#define WEBSERV_RESPONSEHEADER_HPP


#include <string>
#include "Response.hpp"
#include "request.hpp"

class ResponseHeader
{
public:
	// public functions
	ResponseHeader();
	ResponseHeader(const ResponseHeader &src);
	virtual ~ResponseHeader();

	ResponseHeader &operator=(const Response &src);

private:
	// private setter functions
	void 			setAcceptCharsets(const std::string &);
	void 			setAcceptLanguage(const std::string &);
	void 			setAllow(const int &code);
	void 			setAuthorization(const std::string &);
	void 			setContentLanguage();
	void 			setContentLength(int length);
	void 			setContentLocation(const std::string &path, int code);
	void 			setContentType(const std::string &contentType);
	void 			setDate();
	void 			setHost(const std::string &host);
	void 			setLastModified(const std::string &path);
	void 			setLocation(const std::string &path, int code);
	void 			setReferer(const std::string &referer);
	void 			setServer();
	void 			setTransferEncoding();
	void 			setRetryAfter(int code, int number);
	void 			setWwwAuthenticate(int code);

	// private member functions
	std::string		createStatusMessage(int code);
	void 			setAllHeaders(std::string content, std::string path, int code);
	std::string 	writeHeader();
	// public members
public:
	std::string 	getHeader(std::string content, std::string path, int code);



private:
	std::string		_acceptCharsets;
	std::string		_acceptLanguage;
	std::string		_allow;
	std::string		_authorization;
	std::string		_contentLanguage;
	std::string 	_contentLocation;
	std::string		_contentLength;
	std::string		_contentType;
	std::string		_date;
	std::string		_host;
	std::string		_lastModified;
	std::string		_location;
	std::string 	_referer;
	std::string		_retryAfter;
	std::string		_server;
	std::string		_transferEncoding;
	std::string		_wwwAuthenticate;
};


#endif //WEBSERV_RESPONSEHEADER_HPP
