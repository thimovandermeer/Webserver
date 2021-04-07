#ifndef PARSER_HPP
# define PARSER_HPP
# include <string>
# include "location.hpp"
# include "../Cluster/serverCluster.hpp"

bool		isEmptyLine(std::string line);
std::string	firstword(std::string& line);
std::string	trimEndSpaces(std::string line);
void		parseError(int lineNr);
location	*getLocation(std::string &startLine, std::fstream &configFile, int &lineNr);
void		startParsing(std::fstream& configFile, serverCluster *cluster);
void		openConfig(int ac, char **av, serverCluster *cluster);

#endif 
