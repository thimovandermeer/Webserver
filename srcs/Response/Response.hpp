#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <ostream>
# include <cstdio>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <sstream>
# include <sys/stat.h>
# include "responseHeader.hpp"
# include "../Server/server.hpp"
# include "../CGI/CGI.hpp"
# include "../Server/location.hpp"
# include "../Utils/utils.hpp"
# include "../Utils/Base64.hpp"

class Response {
public:
	Response(Request &req, server &serv);
	Response(const Response &src);
	virtual ~Response();
	Response &operator=(const Response &src);

	void 	setupResponse(Request &req, server &serv);

	friend	std::ostream &operator<<(std::ostream &os, const Response &response);

	// new shit by Jonas, needs cleanup
	bool	isFinished;
	int		fileFd;

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
	location					*_currentLoc;
private:
	Response();
	void 		getMethod();
	void 		headMethod();
	void 		postMethod(std::string content);
	void 		putMethod(std::string const &content);
	void		errorPage(server &serv);
    void        createErrorPage(std::string *pageData);
	void 	    readContent();
	int		    authenticate(Request &req);
	void 	    writeContent(std::string const &content);
	void        parseContent();
    std::string headerValue(size_t startPos);




public:
	void				setStatus(int status);
	const std::string 	&getResponse() const;
	size_t				getBodySize() const;
	void 				setCurrentLoc(location *newloc);
	bool				isMethodAllowed();

};

#endif
