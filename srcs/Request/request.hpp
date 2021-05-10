
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <locale>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <fcntl.h>

enum filetype { PY = 1, PHP = 2, BLA = 3, CGIBIN = 4};

class Request{
    public:
		friend class Response;
        ~Request();
        explicit Request(std::string request);
        Request(const Request &original);
        Request &operator=(const Request &original);

        std::string getMethod() const;
        std::string getUri() const;
        std::map<std::string, std::string> getHeaders() const;
        std::map<std::string, std::string> getCgiHeaders() const;
        std::string getBody() const;
        std::string getHost();
        std::string getContentType();
        std::string getCgiEnv() const;
        bool getCgi() const ;
		int getStatus() const;
		filetype getFileType() const;

        void parseRequest();
        void parseRequestLine();
        void parseHeaders();
        void parseBody();
		void checkCgi();

     private:
        Request();
        std::string _request;
        std::string _method;
        std::string _uri;
        std::string _version;
        std::string _cgiEnv;
        std::string _body;
        std::map<std::string, std::string>	_cgiHeaders;
        std::map<std::string, std::string>  _defHeaders;
        int         _status;
        int         _bodyLength;
        int         _contentLength;
        bool        _cgi;
        filetype	_type;
} ;

#endif
