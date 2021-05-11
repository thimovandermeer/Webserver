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


private:
	bool 						_useCGI;
	CGI							_myCGI;
	std::string 				_body;
	std::string 				_response;
	std::string 				_content;
	int 						_status;
	std::string 				_path;
	std::string 				_contentType;
	std::map<int, std::string>	_errorMessage;
	std::string					_method;
	location					*_currentLoc;

private:
	Response();
	void 		postMethod(std::string content);
	void 		putMethod(std::string const &content);
	void		errorPage(server &serv);
    void        createErrorPage(std::string *pageData);
	void 	    readContent();
	int		    authenticate(Request &req);
	void        parseContent();
    std::string headerValue(size_t startPos);
	std::string postContent;
	int			fileFd;
	bool		isFinished;

public:
	void				setStatus(int status);
	const std::string 	&getResponse() const;
	size_t				getBodySize() const;
	void 				setCurrentLoc(location *newloc);
	bool				isMethodAllowed();
	const std::string&	methodType() const;
	const int&			getStatus() const;
	const bool&			getUseCgi() const;
	const bool&			isRespFinished() const;
	CGI&				getCgi();
	std::string&		getBody();

	void	finishread();
	void 	getMethod();
	void 	headMethod();
	void	finishErrorPage(server &serv);
	void	finishPost();
	void	finishPut();
	void	finishPostCgi();
};

#endif
