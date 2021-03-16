//
// Created by Thimo Van der meer on 16/03/2021.
//

#ifndef WEBSERV_GETPATH_HPP
#define WEBSERV_GETPATH_HPP
#include "../Request/request.hpp"
#include "../Response/Response.hpp"

class getPath{
public:
	getPath(server &serv, Request &req, Response &resp);
	getPath(const getPath &src);
	virtual ~getPath();
	getPath &operator=(const getPath &src);
	std::string createPath();
// private members
private:
	server		&_serv;
	Request		&_req;
	Response	&_resp;
	std::string _filePath;
	std::string _rootDir;
	std::string _uri;
	std::string _locMatch;
	size_t		_found;
	location	*_loc;
	bool		_needIndex;
// private functions
private:
	void		locationExists();
	location*	findFileExtension();
	void 		checkPut();
	void checkFile();
// getters
public:
	std::string getFilePath();
	std::string getRootDir();
	void	noLocation();

};

#endif //WEBSERV_GETPATH_HPP
