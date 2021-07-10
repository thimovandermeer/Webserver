#include "webserv.hpp"

void	errMsgAndExit(const std::string &errMsg, int code)
{
	std::cerr << "error: " << errMsg << std::endl;
	exit(code);
}

/*
*	The main function of the entire webserv.
*	We create a servercluster containing all servers
*	And we set up some debugging logs
*/

int main(int argc, char **argv)
{
	serverCluster cluster;
	system("mkdir -p logs && cd logs && rm -f ./*");
	system("rm -f html_pages/Downloads/Download_* html_pages/Downloads/file_should_exist_after html_pages/Downloads/multiple_same");
	openConfig(argc, argv, &cluster);
	cluster.startListening();
	return (0);
}
