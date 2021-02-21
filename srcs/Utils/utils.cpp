//
// Created by Thimo Van der meer on 26/01/2021.
//

#include <sys/time.h>
#include <iostream>

int 	getTime()
{
	struct timeval currentTime;
	int time;
	gettimeofday(&currentTime, NULL);
	time = currentTime.tv_sec;
	std::cout << time << std::endl;
	return time;
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

void	ft_bzero(char *buf, size_t n)
{
    size_t	i;
    char	*ptr;

    ptr = buf;
    i = 0;
    while (i < n)
    {
        ptr[i] = 0;
        i++;
    }
}