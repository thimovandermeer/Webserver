
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <locale>
#include <vector>
#include <algorithm>

enum headerType {
    ACCEPT_CHARSET,
    ACCEPT_LANGUAGE,
    ALLOW,
    AUTHORIZATION,
    CONTENT_LANGUAGE,
    CONTENT_LENGTH,
    CONTENT_LOCATION,
    CONTENT_TYPE,
    DATE,
    HOST,
    LAST_MODIFIED,
    LOCATION,
    REFERER,
    RETRY_AFTER,
    SERVER,
    TRANSFER_ENCODING,
    USER_AGENT,
    WWW_AUTHENTICATE
};

//struct maken met de variabelen van request?
class Request{
    public:
        Request();
        ~Request();
        Request(std::string request);
        Request(const Request &original);
        Request &operator=(const Request &original);

        int getMethod() const;
        std::string getVersion() const;
        std::string getUri() const;
        std::map<headerType, std::string> getHeaders() const;
        std::string getBody() const;
        std::string getContentType();
        std::string getCgiEnv() const;

        void parseRequestLine();
        void parseHeaders();
        void parseBody();

     private:
        std::string _request;
        std::string _method;
        std::string _uri;
        std::string _version;
        std::string _cgiEnv;
        std::map<std::string, headerType>	_headerMap;
        std::map<headerType, std::string>   _defHeaders;
        int         _status;
        bool        _body;
} ;

#endif