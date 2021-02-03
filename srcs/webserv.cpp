//
// Created by Thimo Van der meer on 03/02/2021.
//
#include "../srcs/Cluster/serverCluster.hpp"
#include "../srcs/Request/request.hpp"
#include "../srcs/Response/ResponseHeader.hpp"
#include "../srcs/Response/Response.hpp"
#include "../srcs/Server/server.hpp"
#include "../srcs/Server/location.hpp"
int main(int argc, char **argv)
{
	if (argc == 2)
	{
		serverCluster cluster;

		cluster.startup();
		cluster.startListening();
	}

}