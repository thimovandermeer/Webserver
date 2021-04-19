#ifndef UTILS_HPP
# define UTILS_HPP

void	sigPipeHandler(int signal);
long 	getTime();
void	get_key_value(std::string &str, std::string &key, std::string& value, const char* delim, const char* end);
void	free_array(char **array);
#endif 
