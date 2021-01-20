#include "request.hpp"

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

std::string Request::getMethod() const {
    return _method;
}

std::string Request::getPath() const {
    return _path;
}

std::string Request::getVersion() const {
    return _version;
}

void Request::parseRequest()
{
    size_t      firstSpace;
    size_t      secondSpace;

    firstSpace = _requestLine.find(" ");
    _method = _requestLine.substr(0, firstSpace);
    secondSpace = _requestLine.find(" ",firstSpace+1);
    _path = _requestLine.substr(firstSpace+1, secondSpace-firstSpace-1);
    _version = _requestLine.substr(secondSpace+1, _requestLine.length());

}

//A Request
//-line begins with a method token, followed by a single space, the Request
//-target, another single space, the protocol version and ends with a new line.
//Request
//-line   = method ‘space' Request
//-target ‘space’ HTTP-version ‘newline’.
//example = GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1
//Status-line.






