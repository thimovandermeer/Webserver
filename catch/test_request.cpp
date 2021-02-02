#include "catch.hpp"
#include "../srcs/request.hpp"

TEST_CASE_METHOD(Request, "Request test", "[Request]"){
    std::string req = "POST /files/?fruit.txt HTTP/1.1\r\n"
                      "User_Agent: The Imaginary Browser\r\n\r\n"
                      "hoi\r\n";
    Request test(req);
    test.parseRequest();
    REQUIRE(test.getMethod() == 2);
    REQUIRE(test.getUri() == "/files/");
    REQUIRE(test.getVersion() == "HTTP/1.1");
    REQUIRE(test.getBody() == "hoi\r\n");
    REQUIRE(test.getCgiEnv() == "fruit.txt");
    REQUIRE(test.getContentType() == "NULL");
}

