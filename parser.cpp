#include "webserv.hpp"
#include "server.hpp"
#include "serverCluster.hpp"

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

location	getLocation(std::string &startLine, std::fstream &configFile, int &lineNr)
{
	std::string	line;
	std::string	match;

	match = startLine.substr(9, startLine.length() - 11);
	// 9 is length of "location ", 11 is that + the " {" at the end;
	location	newLoc(match);

	while (std::getline(configFile, line))
	{
		lineNr++;
		if (configFile.eof())
			parseError(lineNr);
		if (isEmptyLine(line))
			continue;
		if (line[0] == '#') // comment line
			continue;
		line = trimEndSpaces(line);
		if (line == "server {" || firstword(line) == "location")
			parseError(lineNr);
		if (line == "}")
			break;
		try
		{
			std::string value = firstword(line);
			newLoc.findValue(value,line);
		}
		catch (std::exception &e)
		{
			std::cerr << "Config file line " << lineNr << ", ";
			leaksExit(e.what(), 1);
		}
	}
	if (!newLoc.valueCheck())
		leaksExit("invalid values in location block", 1);
	return (newLoc);
}

void	startParsing(std::fstream& configFile, serverCluster *cluster)
{
	std::string			line;
	int					lineNr = 0;

	while (std::getline(configFile, line))
	{
		lineNr++;
		if (configFile.eof())
			break;
		if (isEmptyLine(line))
			continue;
		line = trimEndSpaces(line);
		if (line[0] == '#') // comment line
			continue;
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
				continue;
			if (line[0] == '#') // comment line
				continue;
			line = trimEndSpaces(line);
			if (firstword(line) == "location")
			{
				location newLoc = getLocation(line, configFile, lineNr);
				newServer.addLocation(newLoc);
			}
			else if (line == "}") // end of server block
				break;
			else
			{
				// fill server data type
				try
				{
					std::string value = firstword(line);
					newServer.findValue(value,line);
				}

				catch (std::exception &e)
				{
					std::cerr << "Config file line " << lineNr << ", ";
					leaksExit(e.what(), 1);
				}
			}
		}
		// check if all data set in server is correct
		if (!newServer.valueCheck())
			leaksExit("invalid values in server block", 1);
		cluster->addServer(newServer);
	}
	if (cluster->isEmpty())
		leaksExit("config file empty", 1);
}

void openConfig(int ac, char **av, serverCluster *cluster)
{
	std::fstream		configFile;
//	serverCluster		cluster;

	if (ac == 1 || (ac == 2 && g_leaks))
	{
		std::cout << "open default config" << std::endl;
		configFile.open("parseTestConfig.conf");
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
	startParsing(configFile, cluster);
	try
	{
		cluster->startup();
	}
	catch (std::exception &e)
	{
		leaksExit(e.what(), 1);
	}
//	cluster->startListening();
}
