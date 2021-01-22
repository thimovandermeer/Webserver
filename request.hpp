
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>

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
        Request();
        ~Request();
        Request(std::string request);
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
    std::map<headerType, std::string>	_headerMap;
        int         _status;
} ;

#endif