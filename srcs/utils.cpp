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
