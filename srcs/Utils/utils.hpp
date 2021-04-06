#ifndef UTILS_HPP
#define UTILS_HPP

int 	getTime();
void    removeAdjacentSlashes(std::string &str);
int		get_next_line(int fd, std::string& line);
void	get_key_value(std::string &str, std::string &key, std::string& value, const char* delim, const char* end);
#endif 
