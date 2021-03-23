#ifndef WEBSERV_DEFINES_HPP
# define WEBSERV_DEFINES_HPP

// times are in seconds
//# define MAX_LIFETIME 15
# define CONNECTION_TIMEOUT 0 // 0 will make it never time out a connection
# define SELECT_TIMEOUT 2

# define KB 1024
# define MB (KB * 1024)
# define MAXSENDSIZE (1 * MB)
# define MAXREADSIZE (1 * MB)

# define NR_OF_CONNECTIONS 10
# define MAXLOGS 20 // this is to prevent literally 100.000s of log files being created, it will only save the most recent logs
# define PRINTLOG false
# define PRINTOUT false

#endif
