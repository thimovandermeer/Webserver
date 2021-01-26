#include "webserv.hpp"
#include "server.hpp"

bool	isEmptyLine(std::string line)
{
	for (size_t i = 0; line[i]; i++)
	{
		if (!std::isspace(line[i]))
			return (false);
	}
	return (true);
}

std::string	firstword(std::string& line)
{
	std::string ret;
	int			i;

	i = 0;
	while (std::isspace(line[i]))
		i++;
	while (!std::isspace(line[i]) && line[i])
	{
		ret += line[i];
		i++;
	}
	return (ret);
}

std::string	trimEndSpaces(std::string line)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (std::isspace(line[i]))
		i++;
	j = line.length() - 1;
	while (std::isspace(line[j]))
		j--;
	return (line.substr(i, j + 1));
}

void	parseError(int lineNr) // not sure if we wanna exit in this case, can figure it out later
{
	std::stringstream errStr;
	errStr << "Parse error on line " << lineNr;
	leaksExit(errStr.str(), 1);
}

int 	getLocation(std::fstream& configFile, int lineNr)
{
	// should eventually fill a class Location, for now it just skips over
	// all the location lines
	std::string	line;
	while (std::getline(configFile, line))
	{
		lineNr++;
		if (configFile.eof())
			parseError(lineNr);
		std::cout << "line is: " << line << std::endl;
		if (isEmptyLine(line))
			continue;
		line = trimEndSpaces(line);
		if (line == "}")
			break;
	}
	return (lineNr);
}

void	startParsing(std::fstream& configFile)
{
	std::string			line;
	std::vector<server>	serverColleciton;
	int					lineNr = 0;

	while (std::getline(configFile, line))
	{
		lineNr++;
		if (configFile.eof())
			break;
		if (isEmptyLine(line))
			continue;
		std::cout << "line is: " << line << std::endl;
		line = trimEndSpaces(line);
		if (line != "server {")
			parseError(lineNr);
		// start reading in server block
		server	newServer;
		while (std::getline(configFile, line))
		{
			lineNr++;
			if (configFile.eof() && line.length() == 0)
				parseError(lineNr);
			if (isEmptyLine(line))
			{
				continue;
			}
			std::cout << "line is: " << line << std::endl;
			line = trimEndSpaces(line);
			if (firstword(line) == "location")
				lineNr = getLocation(configFile, lineNr); //make and fill location
			else if (line == "}") // end of server block
				break;
			else
				; // fill server data type
		}
		// check if all data set in server is correct
		if (!newServer.valueCheck())
			parseError(lineNr);
		serverColleciton.push_back(newServer);
		std::cout << "done with server block" << std::endl;
	}
	std::cout << "done" << std::endl;
}

void	openConfig(int ac, char **av)
{
	std::fstream configFile;

	if (ac == 1 || (ac == 2 && g_leaks))
	{
		std::cout << "open default config" << std::endl;
		configFile.open("testconfig.conf");
		if (!configFile)
			leaksExit("default config missing", 1);
	}
	else
	{
		configFile.open(av[1]);
		if (!configFile)
		{
			std::string err = "config file \"\" does not exist";
			err.insert(13, av[1]);
			leaksExit(err, 1);
		}
	}
	startParsing(configFile);
}
/*
 * port
 * root
 * optional server_name
 * default error page
 * limit client body size
 * index
 * autoindex
 * host
 * locations
*/