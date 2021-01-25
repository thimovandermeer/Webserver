//
// Created by Thimo Van der meer on 20/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP


#include <string>
#include <ostream>
#include "../srcs/request.hpp"
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
	// functions for each different method
	void 	getMethod();
	void 	headMethod();
	void 	postMethod();
	void 	putMethod(std::string content);
	void 	deleteMethod();
	void 	connectMethod();
	void 	optionsMethod();
	void	traceMethod(Request &request);

	// helper functions
	void 	readContent();
	void 	writeContent(std::string content);


public:
	std::string 		getContent();
	const std::string 	&getResponse() const;
	int 				getCode();

};


#endif //WEBSERV_RESPONSE_HPP
