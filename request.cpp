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

Request::Request(std::string requestLine) : requestLine(_request) {}

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
    size_t pos;

    if (_request[0] == ' ' || _request.find("\r\n") == std::string::npos)
        _status = 400;          //error van maken en eruit gaan //of 301?

    _position = _request.find(" ");
    _method = _request.substr(0, _position);

    pos = _request.find(" ", _position + 1);
    _path = _request.substr(_position, pos);
    _position = _request.find("\r\n");
    _version = _request.substr(pos, _position);
    if (_version.compare("HTTP/1.1") != 0) {        //error overleggen
        _status = 400;
        _version = "NULL";
    }
//    _request = _request.substr(position+1);
}

void Request::parseHeaders()
{
    size_t      position;
}


