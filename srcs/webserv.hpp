#ifndef WEBSERV_HPP
# define WEBSERV_HPP
# include "Cluster/serverCluster.hpp"
# include "../srcs/Request/request.hpp"
# include "CGI/CGI.hpp"
# include "Server/server.hpp"

class	serverCluster;

void	errMsgAndExit(const std::string &errMsg, int code);

void	openConfig(int ac, char **av, serverCluster *cluster);

extern connection	*g_recentConnection;

#endif
