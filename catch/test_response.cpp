#include "catch.hpp"
#include "../srcs/Request/request.hpp"
#include "../srcs/Response/Response.hpp"
#include "../srcs/Response/ResponseHeader.hpp"
#include <ctime>
#include <sys/stat.h>

TEST_CASE_METHOD(Response, "TestGetmethod", "[Getmethod]")
{
	std::string content;
	std::string path;
	int 		code;
	std::string contentType;
	std::string buf;
	content = "Op deze plek staat deze content dus dit moet gereturnt worden he";
	path = "/Users/thimovandermeer/CLionProjects/Webserver/path.txt";
	code = 200;
	contentType = "json";
	ResponseHeader responseHeader(content, path, code, contentType);
	REQUIRE(responseHeader.getAcceptCharsets().empty());
	REQUIRE(responseHeader.getAcceptLanguage().empty());
	REQUIRE(responseHeader.getAllow().empty());
	REQUIRE(responseHeader.getAuthorization().empty());
	REQUIRE(responseHeader.getContentLanguage() == "en-US");
	REQUIRE(responseHeader.getContentLocation() == "/Users/thimovandermeer/CLionProjects/Webserver/path.txt");
	REQUIRE(responseHeader.getContentLength() == "64");
	REQUIRE(responseHeader.getContentType() == "json");
	REQUIRE(responseHeader.getHost().empty());
	REQUIRE(responseHeader.getLastModified() == "Mon, 01 Feb 2021 13:27:32 UTC");
	REQUIRE(responseHeader.getLocation().empty());
	REQUIRE(responseHeader.getReferer().empty());
	REQUIRE(responseHeader.getRetryAfter().empty());
	REQUIRE(responseHeader.getServer() == "Epic Merel Jonas Thimo HTTP Server HUTS");
	REQUIRE(responseHeader.getTransferEncoding() == "Chunked");
	REQUIRE(responseHeader.getWwwAuthenticate().empty());
}

TEST_CASE_METHOD(Response, "TestResponsePutmethod", "[ResponsePut]"){

	Request request("PUT path.txt HTTP/1.1\r\n"
					"Host: eloquentjavascript.net\r\n"
					"Date:leukeserverdit\r\n"
					"Allow: jajaja \r\n"
					"Retry_After: waaromdoejijhetniet\r\n"
					"Content_type: .json\r\n"
					"User_Agent: The Imaginary Browser\r\n\r\n"

					"EPICSICKE SHIUT OUWW\r\n"
					"hoi\r\n"
	);
	request.parseRequest();
	Response 		response;
	RequestConfig	requestConfig;
	std::string path;
	path = "path.txt";
	requestConfig.setpath(path);
	response.checkMethod(request, requestConfig);
	REQUIRE(response.getResponse() == "HTTP/1.1 201 Created
					Content-Language: en-US
					Content-Length: 27
					Content-Location: path.txt
					Content-Type: .json
					Date: Mon, 01 Feb 2021 14:48:30 UTC
					Last-Modified: Mon, 01 Feb 2021 14:48:30 UTC
					Server: Epic Merel Jonas Thimo HTTP Server HUTS
					Transfer-Encoding: Chunked"

	);
	// hier komen functies die goed gaan dus hier moeten specifieke teksten gereturnt worden


}

