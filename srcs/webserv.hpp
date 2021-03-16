#ifndef WEBSERV_HPP
# define WEBSERV_HPP
#include <fstream>
#include "Cluster/serverCluster.hpp"
#include "../srcs/Request/request.hpp"

void	errMsgAndExit(const std::string &errMsg, int code);

void	openConfig(int ac, char **av, serverCluster *cluster);

#endif
