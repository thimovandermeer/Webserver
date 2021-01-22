#include "request.hpp"

std::string methods[8] = {
        "GET",
        "HEAD",
        "POST",
        "PUT",
        "DELETE",
        "CONNECT",
        "OPTIONS",
        "TRACE"
} ;

Request::Request() {}
Request::~Request() {}

Request::Request(const Request &copy)
{
    *this = copy;
}

Request &Request::operator=(const Request &)
{
    return (*this);
}

Request::Request(std::string request) : _request(request) {
    _headerMap[ACCEPT_CHARSET] = "ACCEPT-CHARSET";
    _headerMap[ACCEPT_LANGUAGE] = "ACCEPT_LANGUAGE";
    _headerMap[ALLOW] = "ALLOW";
    _headerMap[AUTHORIZATION] = "AUTHORIZATION";
    _headerMap[CONTENT_LANGUAGE] = "CONTENT_LANGUAGE";
    _headerMap[CONTENT_LENGTH] = "CONTENT_LENGTH";
    _headerMap[CONTENT_LOCATION] = "CONTENT_LOCATION";
    _headerMap[CONTENT_TYPE] = "CONTENT_TYPE";
    _headerMap[DATE] = "DATE";
    _headerMap[HOST] = "HOST";
    _headerMap[LAST_MODIFIED] = "LAST_MODIFIED";
    _headerMap[LOCATION] = "LOCATION";
    _headerMap[REFERER] = "REFERER";
    _headerMap[RETRY_AFTER] = "RETRY_AFTER";
    _headerMap[SERVER] = "SERVER";
    _headerMap[TRANSFER_ENCODING] = "TRANSFER_ENCODING";
    _headerMap[USER_AGENT] = "USER_AGENT";
    _headerMap[WWW_AUTHENTICATE] = "WWW_AUTHENTICATE";
}

int Request::getMethod() const {
    for (int i = 0; i < 7; i++){
        if (_method.compare(methods[i]) == 0)
            return i;
    }
    return -1;      //invalid eerder opvangen?
}

std::string Request::getPath() const {
    return _path;
}

std::string Request::getVersion() const {
    return _version;
}

void Request::parseRequestLine()        //errors overal nog overleggen
{
    size_t pos1;
    size_t pos2;

    if (_request[0] == ' ' || _request.find("\r\n") == std::string::npos)
        _status = 400;          //error van maken en eruit gaan //of 301?
    pos2 = _request.find(" ");
    _method = _request.substr(0, pos2);
    pos1 = _request.find(" ", pos2 + 1);
    _path = _request.substr(pos2+1, pos1-pos2-1);
    pos2 = _request.find("\r\n");
    _version = _request.substr(pos1+1, pos2-pos1-1);
    if (_version.compare("HTTP/1.1") != 0)         //error en return overleggen
        _status = 400;
    _request = _request.substr(pos2+2, _request.length());
    std::cout << _request << "." << std::endl;

}

void Request::parseHeaders()
{
    size_t      pos;
}


