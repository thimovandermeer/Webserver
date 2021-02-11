//
// Created by Thimo Van der meer on 25/01/2021.
//

#ifndef WEBSERV_RESPONSEHEADER_HPP
#define WEBSERV_RESPONSEHEADER_HPP


#include <string>
#include "Response.hpp"
#include "../Request/request.hpp"

class ResponseHeader
{
public:
	// public functions
	ResponseHeader(std::string &content, std::string &path, int status, std::string &contentType);
	ResponseHeader(const ResponseHeader &src);
	virtual ~ResponseHeader();

	ResponseHeader &operator=(const ResponseHeader &src);

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

	// private member functions
	std::string		createStatusMessage(int status);
	std::string 	writeHeader();
	// public members
public:
	std::string getHeader(int status);

	const std::string &getAcceptCharsets() const;

	const std::string &getAcceptLanguage() const;

	const std::string &getAllow() const;

	const std::string &getAuthorization() const;

	const std::string &getContentLanguage() const;

	const std::string &getContentLocation() const;

	const std::string &getContentLength() const;

	const std::string &getContentType() const;

	const std::string &getDate() const;

	const std::string &getHost() const;

	const std::string &getLastModified() const;

	const std::string &getLocation() const;

	const std::string &getReferer() const;

	const std::string &getRetryAfter() const;

	const std::string &getServer() const;

	const std::string &getTransferEncoding() const;

	const std::string &getWwwAuthenticate() const;


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
