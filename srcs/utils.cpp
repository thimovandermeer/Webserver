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

std::string	removeAdjacentSlashes(std::string &str)
{
	std::string	ret;
	bool		lastIsSlash = false;

	for (std::string::size_type i = 0; i < str.length(); i++) {
		if (str[i] == '/') {
			if (!lastIsSlash)
				ret.push_back(str[i]);
			lastIsSlash = true;
		}
		else {
			lastIsSlash = false;
			ret.push_back(str[i]);
		}
	}
	return ret;
}