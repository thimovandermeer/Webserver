#include "Response.hpp"
#include "getPath.hpp"

# define RED			"\x1b[31m"
# define GREEN			"\x1b[32m"

# define END			"\x1b[0m"
# define BOLD			"\x1b[1m"

Response::Response(Request &req, server &serv) :
	_status(req.getStatus()),

	_contentType(req.getContentType()),

	_useCGI(req.getCgi()),
	_method(req.getMethod()),
	_body(req.getBody())
{
	getPath path(serv, req, *this);

	_path = path.createPath();
    CGI cgiTemp(_path, req, serv);
    _CGI = cgiTemp;
    _errorMessage[204] = "No Content";
    _errorMessage[400] = "Bad Request";
    _errorMessage[403] = "Forbidden";
    _errorMessage[404] = "Not Found";
    _errorMessage[405] = "Method Not Allowed";
    _errorMessage[413] = "Payload Too Large";
}

Response::Response()
{

}

Response::Response(const Response &src)
{
	*this = src;
}

Response::~Response()
{
}

void Response::setCurrentLoc(location *newloc)
{
	_currentLoc = newloc;
}

Response	&Response::operator=(const Response &src)
{
	_response = src._response;
	_content = src._content;
	_path = src._path;
	_contentType = src._contentType;
	_useCGI = src._useCGI;
	_status = src._status;
	_errorMessage = src._errorMessage;
	_method = src._method;
	return (*this);
}

bool	Response::isMethodAllowed()
{
	if (!this->_currentLoc)
		return (false);
	std::vector<std::string>::iterator it;
	std::vector<std::string> vc = this->_currentLoc->getMethods();
	for (it = vc.begin(); it < vc.end(); it++)
	{
		if ((*it) == this->_method)
			return (true);
	}
	this->_status = 405;
	return (false);
}

void Response::setupResponse(Request &req, server &serv) {
	if (this->authenticate(req))
	{
		std::cerr << "Authentication failed" << std::endl;
		return;
	}
	if(_method == "GET")
	{
		if (this->isMethodAllowed())
			getMethod();
	}
	else if(_method == "HEAD")
	{
		if (this->isMethodAllowed())
			headMethod(); 
	}
	else if(_method == "POST")
	{
		if (this->isMethodAllowed())
			postMethod(req.getBody());
	}
	else if(_method == "PUT")
	{
		if (this->isMethodAllowed())
			putMethod(_body); 
	}
	if (this->_status >= 299)
	{
		this->errorPage(serv);
	}
}

void 	Response::readContent()
{
	int			fileFd;
	struct stat statBuf;

	if (_useCGI == true)
	{
		this->_content = _CGI.executeGCI(_body);
		return ;
	}
	if(stat(_path.c_str(), &statBuf) != 0)
		return (this->setStatus(404));
	fileFd = open(this->_path.c_str(), O_RDONLY);
	if(fileFd == -1 && this->_status == 200)
		return (this->setStatus(403));
	if(stat(_path.c_str(), &statBuf) != 0 && _status == 200)
		return (this->setStatus(404));
	char buf[statBuf.st_size + 1];
	bzero(buf, statBuf.st_size + 1);
	// break here
	read(fileFd, buf, statBuf.st_size);
	if (this->_status == 200)
		this->_content += buf;
	close(fileFd);
}

void    Response::createErrorPage(std::string *pageData)
{
    size_t found = 1;
	while (found != std::string::npos)
	{
		found = pageData->find("ERROR_CODE");
		if (found == std::string::npos)
			break;
		std::stringstream stat;
		stat << this->_status;
		std::string statstr = stat.str();
		pageData->replace(found, 10, statstr);
	}
	found = 1;
    while (found != std::string::npos)
    {
        found = pageData->find("MESSAGE");
        if (found == std::string::npos)
            break;
        std::map<int, std::string>::iterator it = _errorMessage.find(_status);
        if (it == this->_errorMessage.end())
        	pageData->replace(found, 7, "unknown error");
        else
        	pageData->replace(found, 7, it->second);
    }
}

void	Response::errorPage(server &serv)
{
	int	        fd;
	std::string	pageData;
	std::string	pathToPage;
	struct stat	statBuff;

	pathToPage = serv.getRoot() + serv.getErrorPage();
	if (stat(pathToPage.c_str(), &statBuff) < 0)
	{
		pageData = "Problem serving error.\nError page does not exist.";
	}
	else
	{
		fd = open(pathToPage.c_str(), O_RDONLY);
		if (fd < 0)
			pageData = "Problem serving error.\nCannot open error page.";
		else
		{
			char buf[statBuff.st_size + 1];
			bzero(buf, statBuff.st_size + 1);
			// break here
			read(fd, buf, statBuff.st_size);
			close(fd);
			createErrorPage(&pageData);
		}
	}
	this->_content.clear();
	if (this->_method != "HEAD")
		this->_content = pageData;
	responseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status) + _content;
}

void Response::getMethod()
{
	readContent();
	responseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status) + _content;

}

void Response::headMethod()
{
	readContent();
	responseHeader header(_content, _path, _status, _contentType);
  	_response = header.getHeader(_status);
  	this->_content.clear();
}

std::string Response::headerValue(size_t startPos) {
    size_t pos;
    size_t pos1;
    size_t pos2;

    pos1 = _content.find(' ', startPos);
    pos2 = _content.find(';', startPos);

    pos = pos1 > pos2 ? pos2 : pos1;
    std::string temp = _content.substr(startPos, pos - startPos);
    return temp;
}

void Response::parseContent()
{
    size_t pos;

    if ((pos = _content.find("Status")) != std::string::npos) {
	    std::stringstream ss(headerValue(pos + 8));
	    ss >> _status;
    }
    if ((pos = _content.find("Content-Type")) != std::string::npos)
        _contentType = headerValue(pos + 14);
}

void Response::postMethod(std::string content)
{
	if(_useCGI == true) {
	    int pos;
		readContent();
		parseContent();
		pos = _content.find("\r\n\r\n");
		_content.erase(0, pos + 4);
		responseHeader header(_content, _path, _status, _contentType);
		_response = header.getHeader(_status) + _content;
		return;
	}
	if (this->_currentLoc->getMaxBodySize() < content.length())
		return (this->setStatus(413));
	int	fileFd;
	fileFd = open(_path.c_str(), O_WRONLY | O_APPEND | O_CREAT);
	if(fileFd == -1 && _status == 200)
		this->setStatus(403);
	struct stat statBuf;
	if(stat(_path.c_str(), &statBuf) < 0 && _status == 200)
		this->setStatus(201);
	// break here
	write(fileFd, content.c_str(), content.length());
	close(fileFd);
	content.clear();
	responseHeader header(content, _path, _status, _contentType);
	_response = header.getHeader(_status);
}

void Response::putMethod(std::string const &content)
{
	std::string::iterator it;

	it = _path.end() - 1;
	if ((*it) == '/')
		_path.erase(it);
	writeContent(content);
	responseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status);
}

void 	Response::writeContent(std::string const &content)
{
    int			fileFd;
	struct stat statBuf;

	if(stat(_path.c_str(), &statBuf) < 0 && _status == 200)
		this->setStatus(201);
    fileFd = open(_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT);
	// break here
	write(fileFd, content.c_str(), content.length());
	close(fileFd);
}

const std::string 	&Response::getResponse() const
{
	return _response;
}

void				Response::setStatus(int status)
{
	if (this->_status >= 400)
		return;
	this->_status = status;
}

int					Response::authenticate(Request &req)
{
	if (this->_currentLoc == NULL) {
		return 0;
	}
	if (this->_currentLoc->gethtpasswdpath().empty()) {
		req._defHeaders["AUTHORIZATION"].clear();
		return 0;
	}
	std::string username, passwd, str;
	try {
		std::string auth = req._defHeaders.at("AUTHORIZATION");
		std::string type, credentials;
		get_key_value(auth, type, credentials, " ", "\n\r#;");
		credentials = base64_decode(credentials);
		get_key_value(credentials, username, passwd, ":", "\n\r#;");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "No credentials provided by client" END << std::endl;
	}
	req._defHeaders["AUTHORIZATION"] = req._defHeaders["AUTHORIZATION"].substr(0, req._defHeaders["AUTHORIZATION"].find_first_of(' '));
	req._defHeaders["REMOTE-USER"] = username;
	if (this->_currentLoc->getAuthMatch(username, passwd)) {
		std::cout << GREEN BOLD "Authorization successful!" END << std::endl;
		return 0;
	}

	_status = 401;
	responseHeader header(_content, _path, _status, _contentType);
	_response = header.getHeader(_status);
	return 1;
}

std::ostream &operator<<(std::ostream &os, const Response &response)
{
	os << "_response: " << response._response << " _content: " << response._content << " _path: " << response._path
	   << " _status: " << response._status;
	return os;
}

size_t Response::getBodySize() const
{
	return (this->_content.size());
}

