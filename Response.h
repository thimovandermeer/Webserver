//
// Created by Thimo Van der meer on 20/01/2021.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include <string>
#include <ostream>
#include "request.hpp"
// this needs to be deleted when request is been made
class RequestConfig{
public:
	std::string getpath(){
		return _path;
	};
	void 		setpath(std::string &path){
		_path = path;
	}
private:
	std::string _path;
};
// Dont forget to delete this :)
class Response {
public:
	Response(void);
	Response(const Response &src);
	virtual ~Response();

	Response &operator=(const Response &src);

	void 		checkMethod(Request &request, RequestConfig &requestconfig);

	friend std::ostream &operator<<(std::ostream &os, const Response &response);

	bool operator==(const Response &rhs) const;

	bool operator!=(const Response &rhs) const;

private:
	std::string _response;
	std::string _content;
	std::string _path;
	int 		_code;
private:
	void getMethod();
	void headMethod();
	void postMethod();
	void putMethod();
	void deleteMethod();
	void connectMethod();
	void optionsMethod();
	void traceMethod();
};


#endif //WEBSERV_RESPONSE_H
