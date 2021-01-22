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

Request::Request(std::string requestLine) : _requestLine(requestLine) {}

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
    size_t      position;

    position = _requestLine.find(" ");
    _method = _requestLine.substr(0, position);
    _requestLine = _requestLine.substr(position+1);
    position = _requestLine.find(" ");
    _path = _requestLine.substr(0, position);
    _requestLine = _requestLine.substr(position+1);
    position = _requestLine.find("\n");
    if (_version.compare("HTTP/1.1") != 0) {        //error overleggen
        _error = -1;
        _version = "NULL";
    }
     _version = _requestLine.substr(0, position);
    _requestLine = _requestLine.substr(position+1);
}

void Request::parseHeaders()
{
    size_t      position;
}


