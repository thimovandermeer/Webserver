#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <map>

#define BUFFER_SIZE 42

long 	getTime()
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return (currentTime.tv_sec);
}

void removeAdjacentSlashes(std::string &str)
{
	std::string::iterator it;

	for (it = str.begin(); it < str.end() - 1; it++)
	{
		if ((*it) == '/' && (*(it + 1)) == '/')
			str.erase(it);
	}
}

int	get_next_line(int fd, std::string& line)
{
	int	ret = 1;
	static std::map<int, std::string> m; // m[fd] is my buffer string

	line.clear();
	while (ret > 0) {
		line += m[fd];
		if (line.find('\n') != std::string::npos) {
			m[fd] = line.substr(line.find_first_of('\n') + 1, line.length() - line.find_first_of('\n') - 1);
			line = line.substr(0, line.find_first_of("\r\n"));
			return (ret > 0);
		}
		m[fd].clear();
		char *tmp = (char*) malloc(BUFFER_SIZE + 1);
		if (!tmp)
			return (0);
		for (int i = 0; i < BUFFER_SIZE + 1; i++)
			tmp[i] = 0;
		ret = read(fd, tmp, BUFFER_SIZE);
		m[fd].assign(tmp);
		free(tmp);
	}
	return 0;
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
