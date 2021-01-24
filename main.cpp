#include "request.hpp"

int main(void){
    Request *test = new Request("POST /files/fruit.txt HTTP/1.1\r\n"
                                "Host: eloquentjavascript.net\r\n"
                                "Date:leukeserverdit\r\n"
                                "Allow: jajaja \r\n"
                                "Retry_After: waaromdoejijhetniet\r\n"
                                "User_Agent: The Imaginary Browser\r\n\r\n"
                                "hoi\r\n"
                                "hoi\r\n"
                                );
    test->parseRequestLine();
//    std::cout << test->getMethod() << "." << std::endl ;
//    std::cout << test->getPath() << "." << std::endl ;
//    std::cout << test->getVersion() << "." << std::endl ;
    test->parseHeaders();
    test->parseBody();
//    std::cout << test->getBody() << "." << std::endl;
    delete test;
};
