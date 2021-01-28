// never make changes in catch_main.cpp and catch.hpp
// include in your test.cpp the catch.hpp
// never add the "#define CATCH_CONFIG_MAIN" in another file than this one
// add in the make your test.cpp and the .cpp file with the code you want to test
// make --> run ./catch_test to see the results
// want a more specific result? run ./catch_test -s
// for other options for the executable, check https://github.com/catchorg/Catch2/blob/devel/docs/command-line.md

// how to work with catch2 and classes:
// syntax; TEST_CASE_METHOD(classname, "test description", "[tag]"){
            // create the variable you want to give to the constructor or to an individual function
            // create a variable with the class type and put the variable in it for the constructor
            // if the test first needs to run a certain function, put that function here
            // and now the tests with the required results;
            // REQUIRE(class.function() = required_result)
            // REQUIRE(class.otherfunction() = required_otherresult)
            // }
// example:
//TEST_CASE_METHOD(Request, "Request test", "[Request]"){
        //std::string req = "POST /files/?fruit.txt HTTP/1.1\r\n"
        //                  "User_Agent: The Imaginary Browser\r\n\r\n"
        //                  "hoi\r\n";
        //Request test(req);
        //test.parseRequest();
        //REQUIRE(test.getMethod() == 2);
        //REQUIRE(test.getUri() == "/files/");
        //REQUIRE(test.getContentType() == "NULL");
        //}

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
