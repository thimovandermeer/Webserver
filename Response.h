//
// Created by Thimo Van der meer on 20/01/2021.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include <string>

class Response {
public:
	Response(void);
	Response(const Response &src);
	virtual ~Response();

	Response &operator=(const Response &src);

	void 		checkMethod();
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
