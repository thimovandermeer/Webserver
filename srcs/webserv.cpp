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
	openConfig(argc, argv, &cluster);
	cluster.startListening();
//	system("leaks webserv");
	return (0);
}
