#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include "../webserv.hpp"

//std::pair<int, int>	g_connectionID;

void	sigPipeHandler(int signal)
{
	if (signal != SIGPIPE)
		std::cerr << "truly don't know what the fuck happened here tbh" << std::endl;
	g_recentConnection->resetConnection();
	g_recentConnection->closeConnection();
}

long 	getTime()
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return (currentTime.tv_sec);
}

void	get_key_value(std::string &str, std::string &key, std::string& value, const char* delim, const char* end)
{
	size_t kbegin = str.find_first_not_of(delim);
	size_t kend = str.find_first_of(delim, kbegin);
	key = str.substr(kbegin, kend - kbegin);
	size_t vbegin = str.find_first_not_of(delim, kend);
	size_t vend = str.find_first_of(end, vbegin);
	value = str.substr(vbegin, vend - vbegin);
}

void	free_array(char **array)
{
	int i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
