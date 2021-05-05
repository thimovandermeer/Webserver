#include "webserv.hpp"

void	errMsgAndExit(const std::string &errMsg, int code)
{
	std::cerr << "error: " << errMsg << std::endl;
	exit(code);
}

int main(int argc, char **argv)
{
	serverCluster cluster;
	system("mkdir -p logs && cd logs && rm -f ./*");
	system("rm -f html_pages/Downloads/Download_* html_pages/Downloads/file_should_exist_after html_pages/Downloads/multiple_same");
	openConfig(argc, argv, &cluster);
	cluster.startListening();
	return (0);
}
