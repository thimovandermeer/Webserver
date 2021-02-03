
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

class Request{
    public:
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
		std::string getHost();
        std::string getCgiEnv() const;
		int getStatus() const;

        void parseRequest();
        void parseRequestLine();
        void parseHeaders();
        void parseBody();
		void setMethod(std::string method);
     private:
        Request();
        std::string _request;
        std::string _method;
        std::string _uri;
        std::string _version;
        std::string _cgiEnv;
        std::string _body;
        std::map<std::string, headerType>	_headerMap;
        std::map<headerType, std::string>   _defHeaders;
        int         _status;
        bool        _body_bool;
} ;

#endif