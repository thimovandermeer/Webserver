#include "request.hpp"

int main(void){
    Request *test = new Request("POST /files/fruit.txt HTTP/1.1\r\n"
                                "Host: eloquentjavascript.net\n"
                                "User-Agent: The Imaginary Browser");
    test->parseRequestLine();
    std::cout << test->getMethod() << "." << std::endl ;
    std::cout << test->getPath() << "." << std::endl ;
    std::cout << test->getVersion() << "." << std::endl ;
    delete test;
};
