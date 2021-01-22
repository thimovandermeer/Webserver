#include "request.hpp"

int main(void){
    Request *test = new Request("POST /files/fruit.txt HTTP/1.1\r\n"
                                "Host: eloquentjavascript.net\r\n"
                                "Server: leukeserverdit\r\n"
                                "User-Agent: The Imaginary Browser\r\n\r\n"
                                "hoi");
    test->parseRequestLine();
//    std::cout << test->getMethod() << "." << std::endl ;
//    std::cout << test->getPath() << "." << std::endl ;
//    std::cout << test->getVersion() << "." << std::endl ;
    test->parseHeaders();
    delete test;
};
