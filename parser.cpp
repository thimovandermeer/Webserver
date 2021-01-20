#include "webserv.hpp"

void	startParsing(int ac, char **av)
{
	std::fstream config_file;

	if (ac == 1 || (ac == 2 && g_leaks))
		std::cout << "open default config" << std::endl;// open default config
	else
	{
		config_file.open(av[1]);
		if (!config_file)
		{
			std::string err = "config file \"\" does not exist";
			err.insert(13, av[1]);
			leaksExit(err, 1);
		}
	}
}
/*
 * port
 * host
 * optional server_name
 * first is default
 * default error page
 * limit client body size
 * locations
*/