#ifndef GETPATH_HPP
# define GETPATH_HPP
# include "../Request/request.hpp"
# include "Response.hpp"
# include "../Server/server.hpp"

class getPath{
public:
	getPath(server &serv, Request &req, Response &resp);
	getPath(const getPath &src);
	virtual ~getPath();
	getPath &operator=(const getPath &src);
	std::string createPath();

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

	void		locationExists();
	void 		checkPut();
	void 		checkFile();
	location*	findFileExtension();

public:
	std::string getFilePath();
	std::string getRootDir();
	void		noLocation();

};

#endif 
