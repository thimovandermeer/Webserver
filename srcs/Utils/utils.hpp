//
// Created by Thimo Van der meer on 26/01/2021.
//

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

int 	getTime();
void    removeAdjacentSlashes(std::string &str);
int		get_next_line(int fd, std::string& line);
void	get_key_value(std::string &str, std::string &key, std::string& value, const char* delim, const char* end);
#endif //WEBSERV_UTILS_HPP
