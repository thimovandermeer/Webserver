
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>

class Request{
    public:
        Request();
        ~Request();
        Request(std::string requestLine);
        Request(const Request &copy);
        Request &operator=(const Request &);

        int getMethod() const;
        std::string getVersion() const;
        std::string getPath() const;

        void parseRequestLine();
        void parseHeaders();

     private:
        std::string _request;
        std::string _method;
        std::string _path;
        std::string _version;
        int         _status;
        size_t      _position;
} ;

//A R
//equest-line begins with a method token, followed by a single space, the R
//equest-target, another single space, the protocol version and ends with a new line.
//R
//equest-line   = method ‘space' R
//equest-target ‘space’ HTTP-version ‘newline’.
//example = GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1
//Status-line.

#endif