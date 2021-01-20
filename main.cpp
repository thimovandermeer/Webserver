#include "request.hpp"

int main(void){
    Request *test = new Request("GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1");
    test->parseRequest();
    std::cout << test->getMethod() << "." << std::endl ;
    std::cout << test->getPath() << "." << std::endl ;
    std::cout << test->getVersion() << "." << std::endl ;
    delete test;
};
