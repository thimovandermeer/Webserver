#include "request.hpp"

int main(void){
    Request *test = new Request("POST http://www.w3.org/pub/WWW/TheProject.html HTTP/2.1");
    test->parseRequestLine();
    std::cout << test->getMethod() << "." << std::endl ;
    std::cout << test->getPath() << "." << std::endl ;
    std::cout << test->getVersion() << "." << std::endl ;
    delete test;
};
