#include "request.hpp"

int main() {
	Request request("GET / HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36\r\nSec-Fetch-Dest: document\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.9,nl;q=0.8\r\n\r\n");
	std::cout << request.getMethod() << "." << std::endl;
	std::cout << request.getUri() << "." << std::endl;
	std::cout << request.getVersion() << "." << std::endl;
	// std::cout << request.getBody() << "." << std::endl;
	// std::cout << request.getContentType() << "." << std::endl;
	// std::cout << request.getCgiEnv() << "." << std::endl;
	// std::cout << request.getHost() << "." << std::endl;
	// std::cout << request.getStatus() << "." << std::endl;
	return 0;
}