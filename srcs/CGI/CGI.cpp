#include "CGI.hpp"
#include "../webserv.hpp"

/*
* 	The constructor for CGI saves the Path and the Type of CGI, after that it calls the InitEnvironment function. 
*	So the execution is activated from the constructor
*/

CGI::CGI(std::string &path, Request &request, server &server) :
		_path(path),
		_type(request.getFileType())
{
	_initEnvironment(request, server);
}

CGI::CGI()
{
}

/*
*	Copy constructor which copy's all needed variabels in the current one
*/

CGI::CGI(CGI &src)
{
	if (this != &src)
	{
		this->_environment = src._environment;
		this->_type = src._type;
	}
	return;
}

/*
*	This function clears all the environment variabels
*/

CGI::~CGI() {
    _environment.clear();
}

/*
*	The input filestream is setup using the tmp folder if it fails -1 is being returned
*/

void	CGI::setupIn()
{
	if((this->_fileIn = open("/tmp/fuckyoupeerin.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU)) == -1)
	errMsgAndExit("cgi error", 1);
}


/*
*	In this function all error checking is being executed if something fails the program exits
*/


void CGI::errorCheckChild()
{
	if ((this->_fileOut = open("/tmp/fuckyoupeerout.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU)) == -1)
            errMsgAndExit("cgi error", 1);
        if (dup2(this->_fileOut, STDOUT_FILENO) == -1)
            errMsgAndExit("cgi error", 1);
        if (close(this->_fileOut) == -1)
            errMsgAndExit("cgi error", 1);
		if ((this->_fileIn = open("/tmp/fuckyoupeerin.txt", O_RDONLY, S_IRWXU)) == -1)
            errMsgAndExit("cgi error", 1);
        if(dup2(this->_fileIn, STDIN_FILENO) == -1)
        {
            close(this->_fileIn);
            errMsgAndExit("cgi error", 1);
        }
}

/*
*	In this function all error checks are being performed on the parent side of the pipe
*/
void CGI::errorCheckParent(std::string body)
{
	int retval = write(this->_fileIn, body.c_str(), body.length());
	if (close(this->_fileIn) == -1)
		errMsgAndExit("cgi error", 1);
    if (retval == -1)
        errMsgAndExit("cgi error", 1);
    if ((_pid = fork()) == -1)
        errMsgAndExit("cgi error", 1);

}


/*
*	In this function the actual CGI is being executed 
*/

void CGI::executeGCI(std::string &body)
{
	
	int     status;
    long    executableStart;

	_convertEnv();
    
	errorCheckParent(body);
	
    if (_pid == 0)
	{
		errorCheckChild();
		close(this->_fileIn);
		if(_type == PHP)
		{
			_path = "cgi-bin/php-cgi";
		}
        executableStart = _path.rfind('/') + 1;
		std::string executable = _path.substr(executableStart);
		std::string pathStart = _path.substr(0, executableStart);
		if (chdir(pathStart.c_str()) == -1)
			    errMsgAndExit("cgi error", 1);
		const char *realArgv[2];
		realArgv[0] = executable.c_str();
		realArgv[1] = NULL;
		char *const *argv = const_cast<char *const *>(realArgv);
		int ret = execve(argv[0], reinterpret_cast<char* const*>(argv), _env);
		if (ret < 0)
        {
            free_array(_env);
            errMsgAndExit("cgi error", 1);
        }
    }
	if(waitpid(0, &status, 0) == -1)
	    errMsgAndExit("cgi error", 1);
	free_array(_env);
	if((this->_fileRet = open("/tmp/fuckyoupeerout.txt", O_RDONLY)) == -1)
        errMsgAndExit("cgi error", 1);
}

/*
*	In this function the output from the CGI process is being read and checked for errors
*/

std::string CGI::readOutput()
{
	char buff[MB];
	int readret = 1;
	std::string ret;
	while (readret)
	{
		bzero(buff, MB);
		if ((readret = read(this->_fileRet, buff, MB - 1)) == -1)
			errMsgAndExit("cgi error", 1);
		ret += buff;
	}
	if(close(this->_fileRet) == -1)
		errMsgAndExit("cgi error", 1);
	return (ret);
}

std::string CGI::_setRedirectStatus()
{
	if (_type == PHP)
		return("200");
	else
		return ("CGI");
}

/*
*	This function inits the environment for the CGI process.
*	Its sets all vars accordingly
*/

void CGI::_initEnvironment(Request &request, server &server)
{
    std::stringstream ss;
	std::map<std::string, std::string> cgiHeaders = request.getCgiHeaders();
	std::map<std::string, std::string>::iterator it = cgiHeaders.begin();
	std::map<std::string, std::string> reqHeaders = request.getHeaders();

	if (reqHeaders.find("AUTHORIZATION") != reqHeaders.end())
		this->_environment["AUTH_TYPE"] = reqHeaders["AUTHORIZATION"];
	ss << request.getBody().length();
	this->_environment["CONTENT-LENGTH"] = ss.str();
	ss.clear();
	if (request.getBody().empty())
		this->_environment["CONTENT-TYPE"] = "";
	else
		this->_environment["CONTENT-TYPE"] = request.getMethod();
	this->_environment["GATEWAY_INTERFACE"] = "EPIC CGI";
	this->_environment["PATH_INFO"] = request.getUri() + request.getCgiEnv();
	this->_environment["PATH_TRANSLATED"] = request.getUri();
	this->_environment["REDIRECT_STATUS"] = _setRedirectStatus();
	this->_environment["QUERY_STRING"] = request.getCgiEnv();
	this->_environment["REMOTE_ADDR"] = server.getHost();
	this->_environment["REMOTE_IDENT"] = "Hello this is bullshit so we did not implement this";
	this->_environment["REMOTE_USER"] = "The same as above counts for this";
	this->_environment["REQUEST_METHOD"] = request.getMethod();
	this->_environment["REQUEST_URI"] = request.getUri();
	this->_environment["SCRIPT_NAME"] = request.getUri(); // dit moet wellicht nog anders
	if (_type == PHP)
		this->_environment["SCRIPT_FILENAME"] = "php_tester.php";
	if (reqHeaders.find("HOST") != reqHeaders.end())
		this->_environment["SERVER_NAME"] = reqHeaders["HOST"];
	else
		this->_environment["SERVER_NAME"] = this->_environment["REMOTE_ADDR"];
	ss << server.getPortNr();
	this->_environment["SERVER_PORT"] = ss.str();
	ss.clear();
	this->_environment["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_environment["SERVER_SOFTWARE"] = "Merel Jonas Thimo Epic webserver huts";
    while (it != cgiHeaders.end()) {
        _environment.insert(std::make_pair(it->first, it->second));
        it++;
    }
}

/*
*	This function converts the environment from an std::String to c style string
*/

void CGI::_convertEnv()
{
	std::map<std::string, std::string>::const_iterator it = this->_environment.begin();

	this->_env = new char*[this->_environment.size() + 1];
	if (!_env)
		return ;
	int j = 0;
	while(it != this->_environment.end())
	{
		std::string temp = it->first + "=" + it->second;
		this->_env[j] = strdup(temp.c_str());
		if (!this->_env[j])
			errMsgAndExit("cgi error", 1);
		it++;
		j++;
	}
	_env[j] = NULL;
}
