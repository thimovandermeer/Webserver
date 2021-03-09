#include "request.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>

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
	this->_headerMap = original._headerMap;
	this->_defHeaders = original._defHeaders;
	this->_status = original._status;
	this->_cgi = original._cgi;
    return (*this);
}

Request::Request(std::string request) : _request(request)
{
    _status = 200;
    _chunkedLength = 0;
    _headerMap["ACCEP-CHARSET"] = ACCEPT_CHARSET;
    _headerMap["ACCEPT-LANGUAGE"] = ACCEPT_LANGUAGE;
    _headerMap["ALLOW"] = ALLOW;
    _headerMap["AUTHORIZATION"] = AUTHORIZATION;
    _headerMap["CONTENT-LANGUAGE"] = CONTENT_LANGUAGE;
    _headerMap["CONTENT-LENGTH"] = CONTENT_LENGTH;
    _headerMap["CONTENT-LOCATION"] = CONTENT_LOCATION;
    _headerMap["CONTENT-TYPE"] = CONTENT_TYPE;
    _headerMap["DATE"] = DATE;
    _headerMap["HOST"] = HOST;
    _headerMap["LAST-MODIFIED"] = LAST_MODIFIED;
    _headerMap["LOCATION"] = LOCATION;
    _headerMap["REFERER"] = REFERER;
    _headerMap["RETRY-AFTER"] = RETRY_AFTER;
    _headerMap["SERVER"] = SERVER;
    _headerMap["TRANSFER-ENCODING"] = TRANSFER_ENCODING;
    _headerMap["USER-AGENT"] = USER_AGENT;
    _headerMap["WWW-AUTHENTICATE"] = WWW_AUTHENTICATE;
    _headerMap["REMOTE_USER"] = REMOTE_USER;
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

std::map<headerType, std::string> Request::getHeaders() const {
    return _defHeaders;
}

std::string Request::getBody() const {
    return _body;
}

std::string Request::getContentType()  {
    if (_defHeaders.begin() == _defHeaders.end())
        return ("NULL");
    std::map<headerType, std::string>::iterator it = _defHeaders.find(CONTENT_TYPE);
    if (it == _defHeaders.end())
        return ("NULL");
    return (it->second);
}

std::string Request::getCgiEnv() const{
    return _cgiEnv;
}

int Request::getStatus() const {
	return _status;
}

void Request::parseRequest() {
    checkCGI();
    parseRequestLine();
    parseHeaders();
    std::map<headerType, std::string>::iterator it = _defHeaders.find(TRANSFER_ENCODING);
    if (it != _defHeaders.end()) {
        if (it->second.compare("chunked") == 0)
            parseBody();
    }
    else
        _body = _request.substr(0, _request.length() - 2);
    _request.clear();
}

void Request::checkCGI() {
    if (_request.find(".py") != std::string::npos)
        _cgi = true;
    else if (_request.find(".php") != std::string::npos)
        _cgi = true;
    else if (_request.find(".bla") != std::string::npos)
        _cgi = true;
    else if(_request.find(".cgi") != std::string::npos)
        _cgi = true;
    else if (_request.find("cgi-bin") != std::string::npos)
        _cgi = true;
    else
        _cgi = false;
}

void Request::parseRequestLine(){
    size_t      pos1;
    size_t      pos2;

    if (_request[0] == ' ' || _request.find("\r\n") == std::string::npos)
        _status = 400;
    pos2 = _request.find(" ");
    _method = _request.substr(0, pos2);
    if (getMethod().empty()){
        _status = 400;
    }
    pos2+=1;
    pos1 = _request.find(" ", pos2);
	size_t qMarkLocation = _request.find("?", pos2); // is npos if no '?'
    if (qMarkLocation <= pos1){
        pos1 = _request.find("?");
        _cgi = true;
        _uri = _request.substr(pos2, pos1-pos2);
        pos2 = _request.find(" ", pos1);
        _cgiEnv = _request.substr(pos1+1, pos2-pos1-1);		//ook checken
    }
    else
	{
		_uri = _request.substr(pos2, pos1-pos2);
		pos2 = _request.find(" ", pos1);
	}
    pos1 = _request.find("\r\n");
	pos2++;
    _version = _request.substr(pos2, pos1-pos2);
    if (_version.compare("HTTP/1.1") != 0)
        _status = 400;
    _request = _request.substr(pos1+2);
}

void Request::parseHeaders() {
    size_t      pos = 0;
    size_t      length;
    std::string header;
    std::string upperHeader;
    std::string value;
	bool loop = true;

    while (loop == true){
        header.clear();
        value.clear();
        upperHeader.clear();
        if (_request.find(":", pos) == std::string::npos) {
            _status = 400;
            return ;
        }
        length = _request.find(":", pos);
        if (std::isspace(_request[length - 1])){       //spatie voor :
            _status = 400;
            return ;
        }
        header = _request.substr(pos, length-pos);
        if (std::isspace(_request[length + 1]))     //spatie na :
            pos = length+2;
        else
            pos = length+1;
        length = _request.find("\r\n", pos);
        value = _request.substr(pos, length-pos);
        for (int i = 0; header[i]; i++)
            upperHeader += std::toupper(header[i]);
        std::map<std::string, headerType>::iterator it = _headerMap.find(upperHeader);
        if (it == _headerMap.end())
		{
			pos = length+2;			//van hier tot 
			if (_request[pos] == '\r' && _request[pos + 1] == '\n')
				loop = false;
        	continue;				// hier eruithalen of beter behandelen
			_status = 400;
			return;
		}
        std::map<headerType, std::string>::iterator it_h = _defHeaders.find(it->second);
        if (it_h != _defHeaders.end())
		{
			_status = 400;
			return;
		}
        _defHeaders.insert(std::make_pair(it->second, value));
        pos = length+2;
		if (_request[pos] == '\r' && _request[pos + 1] == '\n')
			loop = false ;
    }
	if (_defHeaders.empty())
	{
		_status = 400;			//als er helemaal geen geldige headers zijn binnengekomen
		return ;
	}	
    _request = _request.substr(pos+2);
}

void Request::parseBody() {
    size_t begin = 0;
    size_t end;
    size_t last = _request.rfind("\r\n");
    std::string hex;
    _body = "";
    if (_request.compare("0\r\n\r") == 0)
        return ;
    while (begin != last - 2){          //voor tester -0 ?
        end = _request.find("\r\n", begin);
        hex = _request.substr(begin, end - begin);
        _chunkedLength += stoi(hex, 0, 16);
        hex.clear();
        begin = end + 2;
        end = _request.find("\r\n", begin);
        _body.append(_request, begin, end - begin);
        begin = _request.find("\r\n", end + 2);
    }
}

bool Request::getCgi() const {
	return _cgi;
}

size_t Request::getChunkedLength() const {
    return _chunkedLength;
}