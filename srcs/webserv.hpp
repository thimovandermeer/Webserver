#ifndef WEBSERV_HPP
# define WEBSERV_HPP
# include "Cluster/serverCluster.hpp"

void	errMsgAndExit(const std::string &errMsg, int code);

void	openConfig(int ac, char **av, serverCluster *cluster);

#endif
