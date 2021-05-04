#ifndef WEBSERV_DEFINES_HPP
# define WEBSERV_DEFINES_HPP

# define CONNECTION_TIMEOUT 0 // 0 will make it never time out a connection
# define SELECT_TIMEOUT 2

# define KB 1024
# define MB (KB * 1024)
# define MAXSENDSIZE (7 * MB)
# define MAXREADSIZE (7 * MB)

# define NR_OF_CONNECTIONS 200
# define MAXLOGS 20 // this is to prevent literally 100.000s of log files being created, it will only save the most recent logs

//# define PRINTOUT
//# define PRINTLOG

#endif
