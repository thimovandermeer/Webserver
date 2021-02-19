#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP

# include <string>
# include <ostream>
# include "../Request/request.hpp"
# include "../Server/server.hpp"
// this needs to be deleted when request is been made

class Response {
public:
	Response(void);		//kan private?
	Response(const Response &src);
	virtual ~Response();

	Response &operator=(const Response &src);

	void 	setupResponse(Request &request, server &server);

	friend std::ostream &operator<<(std::ostream &os, const Response &response);

	bool operator==(const Response &rhs) const;

	bool operator!=(const Response &rhs) const;

private:
	std::string 				_response;
	std::string 				_content;
	std::string 				_path;
	std::string 				_contentType;
	int 						_status;
	std::map<int, std::string>	_errorMessage;
	
private:
	// functions for each different method
	void 		getMethod();
//	std::string	getPath(server &server, Request &request);
	void 		headMethod();
	void 		postMethod(std::string content);
	void 		putMethod(std::string content);

	void		errorPage(server &serv);
    void        createErrorPage(std::string *pageData);
        // helper functions
	void 	readContent();
	void 	writeContent(std::string content);


public:
	void				setStatus(int status);
	std::string 		getContent();
	const std::string 	&getResponse() const;
	int 				getCode();

};

std::string	getPath(server &server, Request &request, Response &response);


#endif //WEBSERV_RESPONSE_HPP
