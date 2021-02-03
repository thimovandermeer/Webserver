//
// Created by Thimo Van der meer on 20/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP


#include <string>
#include <ostream>
#include "../Request/request.hpp"

// this needs to be deleted when request is been made

class Server {
public:
	std::string _root;
	std::string 		getRoot() { return _root;};
	void		 		setRoot(std::string &root) {_root = root;};
};
class Response {
public:
	Response(void);
	Response(const Response &src);
	virtual ~Response();

	Response &operator=(const Response &src);

	void 		checkMethod(Request &request, Server &server);

	friend std::ostream &operator<<(std::ostream &os, const Response &response);

	bool operator==(const Response &rhs) const;

	bool operator!=(const Response &rhs) const;

private:
	std::string _response;
	std::string _content;
	std::string _path;
	std::string _contentType;
	int 		_code;
private:
	// functions for each different method
	void 		getMethod();
	std::string	getPath(Server &server, Request &request);
	void 		headMethod();
	void 		postMethod(std::string content);
	void 		putMethod(std::string content);

	// helper functions
	void 	readContent();
	void 	writeContent(std::string content);


public:
	std::string 		getContent();
	const std::string 	&getResponse() const;
	int 				getCode();

};


#endif //WEBSERV_RESPONSE_HPP
