
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>

class Request{
    public:
        Request();
        ~Request();
        Request(std::string RequestLine);
        Request(const Request &copy);
        Request &operator=(const Request &);

        std::string getMethod() const;
        std::string getVersion() const;
        std::string getPath() const;

        void parseRequest();


     private:
        std::string _requestLine;
        std::string _method;
        std::string _path;
        std::string _version;

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