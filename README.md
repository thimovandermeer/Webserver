# Webserver
This is our webserver project

unlink, exit, lseek,  gettimeofday, strptime, strftime, usleep, select, socket, accept, listen, send, recv, bind, connect, inet_addr, setsockopt, getsockname, fcntl

## Time:
### usleep():
Usage: _int usleep(useconds_t microseconds);_

This function suspends execution of the calling thread until either the microseconds have elapsed or a signal is deliverd to the thread and it action is to invoke a signal-catching function or to terminate the process.

### gettimeofday():
Usage: _int gettimeofday(struct timeval *restrict tp, void *restrict tzp);_

Gives the system's notion of the current Greenwich time.
truct timeval: 
{
	time_t       tv_sec;   /* seconds since Jan. 1, 1970 */
	suseconds_t  tv_usec;  /* and microseconds */
};
The function returns a 0 when the call succeeded, a -1 is returned when an error occured and the errno is set to indicate the error. 
Depending on whether tp or tzp is NULL, one of the structs is populated with the timezone struct.

## Directory handling:
- mkdir, rmdir
### getcwd():
Usage: _char *getcwd(char *buf, size_t size);_

Get current working directory.
getcwd mallocs for _buf_ for length _size_. If _size_ is 0, _buf_ is allocated as big as necessary. Returned buffer has to be freed!

### chdir():
Usage: _int chdir(const char *path);_

Change working directory.
Returns 0 on success and -1 on error.

### opendir():
Usage: _DIR *opendir(const char *name);_

Opens the directory stream of the provided _path_.
Return pointer to the directory stream or NULL on error.

### readdir():
Usage: _struct dirent *readdir(DIR *dirp);_

Reads a directory stream.
Returns a pointer to _dirent struct_ containing the next item in DIR or NULL on error.	 
The struct has the following format:
```
struct dirent {
    ino_t          d_ino;       /* inode number */
    off_t          d_off;       /* offset to the next dirent */
    unsigned short d_reclen;    /* length of this record */
    unsigned char  d_type;      /* type of file; not supported
                                   by all file system types */
    char           d_name[256]; /* filename */
};
```

### closedir():
Usage: _int closedir(DIR *dirp);_

Closes the directory stream associated with \*dirp.
Return 0 on success and -1 on error.

## File handling:
- open, read, close

### dup() & dup2():
Usage:
- _int dup(int oldfd);_
- _int dup2(int oldfd, int newfd);_

Both system calls duplicate an open filediscriptor. 
Dup chooses the lowest available fd for the duplicate whereas dup2 let's you specify the new fd. 
Afterwards, both fd's can be used interchangebaly. 

On success, these system calls return the new descriptor. On error, -1 is returned, and errno is set appropriately. 

### stat() & lstat() & fstat():
Usage:
- _int stat(const char *restrict path, struct stat *restrict buf);_
- _int lstat(const char *restrict path, struct stat *restrict buf);_
- _int fstat(int fildes, struct stat *buf);_

The stat() function obtains information about the file pointed to by _path_. The lstat() function is like stat() except in the case where the named file is a symbolic link; lstat() returns information about the link, while stat() returns information about the file the link references.
The fstat() obtains the same information about an open file known by thefile descriptor _fildes_.

Upon successful completion a value of 0 is returned. Otherwise, a value of -1 is returned and errno is set to indicate the error.

## Wait, process termination.
Good to know:
- pid_t is a data type with a signed integer type which is capable of representing a process ID.

### wait():
Usage: _pid_t wait(int *stat_loc);_

Suspends execution of its calling process until stat_loc information is available for a terminated child process, or a signal is received. 

If wait() returns due to a stopped or terminated child process, the process ID of the child is returned to the calling process (saved in stat_loc).  Otherwise, a value of -1 is returned and errno is set to indicate the error. If any process has no child process, then wait() immediately returns -1.

### wait4() & wait3() & waitpid():
Usage: 
- _pid_t wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);_
- _pid_t wait3(int *stat_loc, int options, struct rusage *rusage);_
- _pid_t waitpid(pid_t pid, int *stat_loc, int options);_

The _pid_ paramater specifies the set of child processes for which to wait. 

_stat_loc_ is a pointer to an area where status information about how the child process ended is to be placed.

If _rusage_ is non-zero, a summary of the resources used by the terminated process and all its children is returned (this information is currently not available for stopped processes).
     
Wait4() provides a more general interface for programs that need to wait for certain child processes, that need resource utilization statistics accumulated by child processes. The older wait3() call is the same as wait4() with a pid value of -1. The waitpid() call is identical to wait4() with an rusage value of zero.

## Other

### execve():
Usage: _int execve(const char *pathname, char *const argv[], char *const envp[]);_

Executes the program referred to by _pathname_.  This causes
the program that is currently being run by the calling process to be
*replaced with a new program*, with newly initialized stack, heap, and
(initialized and uninitialized) data segments.

### exit():
Usage: _void exit(int status)_

Status is the status value returned to the parent process.

### fork():
Usage: _pid_t fork(void);_

Causes creation of a new process, The new process (child process) is an exact copy of the calling process (parent process).

Upon succesful completion, fork() returns a value of 0 to the child process and returns the process ID of the child process to the parent process. If it isn't successful, a value of -1 is returned to the parent process, no child process is created, and the global variable eerno is set to indicate the error.

### kill():
usage: _int kill(pid_t pid, int sig);_

Sends a signal to a process or a group of processes specified by pid. The signal to be sent is specified by sig.

### pipe():
Usage: _int pipe(int pipefd[2]);_

Creates a pipe, a unidirectional data channel that can be used for interprocess communication. The array pipefd is used to return two file descriptors referring to the ends of the pipe. pipefd[0] refers to the read end of the pipe. pipefd[1] refers to the write end of the pipe. Data written to the write end of the pipe is buffered by the kernel until it is read from the read end of the pipe.

Return 0 on success and -1 on error.

### signal():
Usage: _void (*signal(int sig, void (*func)(int)))(int);_
Or, in the equivalent but easier to read:
_typedef void (*sig_t) (int);_
_sig_t signal(int sig, sig_t func);_

Signals allow the manipulation of a process from outside its domain, as well as allowing the process to manipulate itself or copies of itself (children).  There are two general types of signals: those that cause termination of a process and those that do not.

The _sig_ arguments specifies which signal was received(there are 31 options). The _func_ procedure allows a user to choose the action upon receipt of a signal. 

### strerror():
Usage: _char *strerror(int errnum);_
Returns a string describing error number _errnum_.

### errno:
Errno is a variable that contains the error number of the last error.

