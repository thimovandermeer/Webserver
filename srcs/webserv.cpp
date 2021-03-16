#include "webserv.hpp"

void	errMsgAndExit(const std::string &errMsg, int code)
{
	std::cerr << "error: " << errMsg << std::endl;
	if (errno)
		perror(NULL);
	exit(code);
}

int main(int argc, char **argv)
{
	serverCluster cluster;
	system("cd logs && rm -f ./*");
	openConfig(argc, argv, &cluster);
	cluster.startListening();
}