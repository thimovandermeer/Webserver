#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP
# include <string>
# include <ostream>
# include "../Request/request.hpp"
# include "../Server/server.hpp"
# include "../CGI/CGI.hpp"
# include <cstdio>

// this needs to be deleted when request is been made

class Response {
public:
	Response(Request &req, server &serv);
	Response(const Response &src);
	virtual ~Response();
	Response &operator=(const Response &src);

	void 	setupResponse(Request &req, server &serv);

	friend std::ostream &operator<<(std::ostream &os, const Response &response);

private:
	std::string 				_response;
	std::string 				_content;
	int 						_status;
	std::string 				_path;
	std::string 				_contentType;
	CGI							_CGI;
	bool 						_useCGI;
	std::map<int, std::string>	_errorMessage;
	std::string					_method;
	std::string 				_body;
private:
	Response();
	// functions for each different method
	void 		getMethod();
	void 		headMethod();
	void 		postMethod(std::string content);
	void 		putMethod(std::string const &content);
	void		errorPage(server &serv);
    void        createErrorPage(std::string *pageData);
		// helper functions
	void 	    readContent();
	int		    authenticate(Request &req);
	void 	    writeContent(std::string const &content);
	void        parseContent();
    std::string headerValue(size_t startPos);

    location			*currentLoc;


public:
	void				setStatus(int status);
	const std::string 	&getResponse() const;
	bool				isMethodAllowed();

	friend std::string	getPath(server &serv, Request &req, Response &resp);

};

#endif //WEBSERV_RESPONSE_HPP
