//
// Created by Thimo Van der meer on 25/01/2021.
//

#ifndef WEBSERV_RESPONSEHEADER_HPP
#define WEBSERV_RESPONSEHEADER_HPP


#include <string>
#include "Response.hpp"
#include "../Request/request.hpp"

class responseHeader
{
public:
	// public functions
	responseHeader(std::string &content, std::string &path, int status, std::string &contentType);
	responseHeader(const responseHeader &src);
	virtual ~responseHeader();

	responseHeader &operator=(const responseHeader &src);

private:
	// private setter functions
	void 			setAllow(const int &status);
	void 			setContentLanguage();
	void 			setContentLength(int length);
	void 			setContentLocation(const std::string &path, int status);
	void 			setContentType(const std::string &contentType);
	void 			setDate();
	void 			setLastModified(const std::string &path);
	void 			setLocation(const std::string &path, int status);
	void 			setServer();
	void 			setTransferEncoding();
	void 			setRetryAfter(int status, int number);
	void 			setWwwAuthenticate(int status);
	void 			resetValues(void);

	// private member functions
	std::string		createStatusMessage(int status);
	std::string 	writeHeader();
	// public members
public:
	std::string getHeader(int status);


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
