#include "request.hpp"
#include <sstream>

std::string methods[4] = {
        "GET",
        "HEAD",
        "POST",
        "PUT",
} ;

Request::Request() {}
Request::~Request() {}

Request::Request(const Request &original) {
    *this = original;
}

Request &Request::operator=(const Request &original) {
    this->_request = original._request;
    this->_method = original._method;
    this->_uri = original._uri;
    this->_version = original._version;
	this->_cgiEnv = original._cgiEnv;
	this->_body = original._body;
	this->_defHeaders = original._defHeaders;
	this->_status = original._status;
	this->_cgi = original._cgi;
    return (*this);
}

Request::Request(std::string request) : _request(request)
{
    _bodyLength = 0;
    _contentLength = -1;
    _status = 200;
	parseRequest();
}

std::string Request::getMethod() const {
    for (int i = 0; i < 4; i++){
        if (_method.compare(methods[i]) == 0)
            return methods[i];
    }
    std::string s;
    return s;
}

std::string Request::getUri() const {
    return _uri;
}

std::map<std::string, std::string> Request::getHeaders() const {
    return _defHeaders;
}

std::map<std::string, std::string> Request::getCgiHeaders() const {
    return _cgiHeaders;
}

std::string Request::getBody() const {
    return _body;
}

std::string Request::getContentType()  {
    if (_defHeaders.begin() == _defHeaders.end())
        return ("NULL");
    std::map<std::string, std::string>::iterator it = _defHeaders.find("CONTENT-TYPE");
    if (it == _defHeaders.end())
        return ("NULL");
    return (it->second);
}

std::string Request::getHost() {
    if (_defHeaders.begin() == _defHeaders.end()){
        _status = 400;
        return ("NULL");
    }
    std::map<std::string, std::string>::iterator it = _defHeaders.find("HOST");
    if (it == _defHeaders.end()){
        _status = 400;
        return ("NULL");
    }
    std::string host = it->second;
    std::size_t found = host.find(":");
        if(found != std::string::npos)
            host = host.substr(0, found);
    return (host);
}

std::string Request::getCgiEnv() const{
    return _cgiEnv;
}

int Request::getStatus() const {
	return _status;
}

filetype Request::getFileType() const
{
	return _type;
}

void Request::parseRequest() {
    checkCgi();
    parseRequestLine();
    parseHeaders();
    std::map<std::string, std::string>::iterator it = _defHeaders.find("TRANSFER-ENCODING");
    if (it != _defHeaders.end()) {
        if (it->second.compare("chunked") == 0)
            parseBody();
    }
    else
        _body = _request.substr(0, _request.length() - 2);
    _request.clear();
}

void Request::checkCgi() {
	if (_request.find(".py") != std::string::npos)
	{
		_cgi = true;
		_type = PY;
	}
	else if (_request.find(".php") != std::string::npos)
	{
		_cgi = true;
		_type = PHP;
	}
	else if (_request.find(".bla") != std::string::npos)
	{
		_cgi = true;
		_type = BLA;
	}
	else if(_request.find(".cgi") != std::string::npos)
	{
		_cgi = true;
		_type = CGIBIN;
	}
	else if (_request.find("cgi-bin") != std::string::npos)
	{
		_cgi = true;
		_type = CGIBIN;
	}
	else
		_cgi = false;
}

void Request::parseRequestLine(){
    size_t      pos1;
    size_t      pos2;

    if (_request[0] == ' ' || _request.find("\r\n") == std::string::npos)
        _status = 400;
    pos2 = _request.find(' ');
    _method = _request.substr(0, pos2);
    if (getMethod().empty()){
        _status = 400;
    }
    pos2+=1;
    pos1 = _request.find(' ', pos2);
	size_t qMarkLocation = _request.find('?', pos2);
    if (qMarkLocation <= pos1){
        pos1 = _request.find('?');
        _cgi = true;
        _uri = _request.substr(pos2, pos1-pos2);
        pos2 = _request.find(' ', pos1);
        _cgiEnv = _request.substr(pos1+1, pos2-pos1-1);
    }
    else
	{
		_uri = _request.substr(pos2, pos1-pos2);
		pos2 = _request.find(' ', pos1);
	}
    pos1 = _request.find("\r\n");
	pos2++;
    _version = _request.substr(pos2, pos1-pos2);
    _request = _request.substr(pos1+2);
}

void Request::parseHeaders() {
    size_t                                          pos = 0;
    size_t                                          length;
    std::string                                     header;
    std::string                                     upperHeader;
    std::string                                     value;
    std::map<std::string, std::string>::iterator    it;
	bool                                            loop = true;

    while (loop == true){
        header.clear();
        value.clear();
        upperHeader.clear();
        if (_request.find(":", pos) == std::string::npos) {
            _status = 400;
            return ;
        }
        length = _request.find(":", pos);
        if (std::isspace(_request[length - 1])){      
            _status = 400;
            return ;
        }
        header = _request.substr(pos, length-pos);
        if (std::isspace(_request[length + 1]))   
            pos = length+2;
        else
            pos = length+1;
        length = _request.find("\r\n", pos);
        value = _request.substr(pos, length-pos);
        if (header[0] == 'X' && header[1] == '-')
        {
        	std::string insert("HTTP_");
			std::replace(header.begin(), header.end(), '-', '_');
            insert.append(header);
			_cgiHeaders.insert(std::make_pair(insert, value));
            pos = length+2;
            if (_request[pos] == '\r' && _request[pos + 1] == '\n')
                loop = false ;
            continue ;
        }
        for (int i = 0; header[i]; i++)
            upperHeader += std::toupper(header[i]);
        it = _defHeaders.find(upperHeader);
        if (it != _defHeaders.end())
		{
			_status = 400;
			return;
		}
        _defHeaders.insert(std::make_pair(upperHeader, value));
        pos = length+2;
		if (_request[pos] == '\r' && _request[pos + 1] == '\n')
			loop = false ;
    }
	if (_defHeaders.empty())
	{
		_status = 400;
		return ;
	}
	it = _defHeaders.begin();
	while (it != _defHeaders.end())
    {
        if (it->first.compare("CONTENT-LENGTH") == 0) {
	    std::stringstream ss;
	    ss << std::dec << it->second.c_str();
            ss >> _contentLength;
            break;
        }
        it++;
    }
    _request = _request.substr(pos+2);
}


void Request::parseBody() {
    size_t begin = 0;
    size_t end;
    std::string hex;
    size_t last = _request.rfind("\r\n");
    _body = "";
    if (_request.compare("0\r\n\r\n") == 0)
        return ;
    while (begin != last - 2){
        end = _request.find("\r\n", begin);
        hex = _request.substr(begin, end - begin);
	std::stringstream ss;
	int tmp;
	ss << std::hex << hex.c_str();
	ss >> tmp;
	_bodyLength += tmp;
        begin = end + 2;
        end = _request.find("\r\n", begin);
        _body.append(_request, begin, end - begin);
        begin = _request.find("\r\n", end + 2);
        hex.clear();
    }
    if (_bodyLength != _contentLength && _contentLength != -1)
        _status = 413;
}

bool Request::getCgi() const {
	return _cgi;
}
