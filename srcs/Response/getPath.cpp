#include "getPath.hpp"


getPath::getPath(server &serv, Request &req, Response &resp) :
	_serv(serv),
	_req(req),
	_resp(resp)
{
}

getPath::~getPath()
{

}
location*	getPath::findFileExtension()
{
	std::vector<location*> locs = _serv.getLocations();

	for (std::vector<location*>::iterator it = locs.begin(); it < locs.end(); it++)
	{
		if ((*it)->isFileExtension())
		{
			std::string	extension = (*it)->getMatch();
			if (extension == "*.error_image.png") 
				extension.erase(0, 2);
			else if (extension == "*.php")
				extension.erase(0,2);
			else
				extension.erase(0, 1);
			size_t len = extension.length();
			if (_uri.length() >= len && !_uri.compare(_uri.length() - len, len, extension))
			{
				if (extension == "error_image.png")
					_uri = "/error_image.png";
				else if (extension == "php")
					_uri = "/cgi/bin/php-cgi";
				return (*it);
			}
		}
	}
	return (NULL);
}

void	getPath::noLocation()
{
	if (_uri.find('.') != std::string::npos) // file requested
		checkFile();
	else // location index requested
	{
		_needIndex = true;
		if (_uri == "/")
			_locMatch = "/";
		else {
			_found = _uri.find_first_of('/', 1);
			_locMatch = _uri.substr(0, _found);
			if (_found != std::string::npos)
				_uri.erase(0, _found + 1);
			else
				_uri.erase(0, _found);
			if (_uri.length() && _uri[_uri.length() - 1] != '/' && _uri != "/") // add '/' at end
				_uri += "/";
		}
	}
}

void	getPath::checkFile()
{
	_found = _uri.find_first_of('/', 1);
	if (_uri.find_last_of('/') == 0) {
		_locMatch = "/";
		_uri.erase(0, 1);
	} else
		_locMatch = _uri.substr(0, _found);
	if (_uri.length() > 1 && _locMatch != "/")
	{
		if (_found != std::string::npos)
			_uri.erase(0, _found + 1);
		else
			_uri.erase(0, _found);
	}
}

void	getPath::locationExists()
{
	struct stat statBuf = {};
	if (!_loc->getRoot().empty()) // location has no own root, so we use the server root
		_rootDir = _loc->getRoot();
	else
		_rootDir = _serv.getRoot();

	if (!_loc->getCgiPath().empty()) 
	{
		_filePath = _rootDir + _loc->getCgiPath();
		_resp.setCurrentLoc(_loc); 
		return ;
	}
	if (_needIndex && _req.getMethod().compare("PUT") != 0 && this->_loc->getAutoindex())
		checkPut();
	else
	{
		if (_req.getFileType() == PHP)
		{
			_filePath = _rootDir;
			return ;
		}
		_filePath = _rootDir + _uri;
		if (stat(_filePath.c_str(), &statBuf) != 0 && _req.getMethod().compare("PUT") != 0)
			_resp.setStatus(404);
	}
}

void getPath::checkPut()
{
	std::vector<std::string>	indices;
	std::vector<std::string>::iterator it;

	struct stat statBuf = {};
	if (!this->_loc->getIndices().empty())
		indices = this->_loc->getIndices();
	else
		indices = this->_serv.getIndices();
	for (it = indices.begin(); it < indices.end(); it++)
	{
		if (this->_uri != "/")
			this->_filePath = this->_rootDir + this->_uri + (*it);
		else
			this->_filePath = this->_rootDir + (*it);
		if (stat(this->_filePath.c_str(), &statBuf) == 0)
			break;
	}
	if (it == indices.end()) // all index pages don't exist at requested root
		this->_resp.setStatus(404);
}

std::string	getPath::createPath()
{
	_loc = NULL;
	_needIndex = false;
	_uri = _req.getUri();
	_loc = findFileExtension();
	if (!_loc)
	{
		noLocation();
		_loc = _serv.findLocation(_locMatch);
	}
	else
		_uri.erase(0, 1);
	if (!_loc && _req.getMethod().compare("PUT") != 0)
	{
		_resp.setStatus(404); 
		_resp.setCurrentLoc(_loc);
		return ("");
	}
	if(_req.getMethod().compare("POST") == 0 &&  _loc->getCgiPath().empty())
	{
		if (!_loc->getRoot().empty()) // location has no own root, so we use the server root
			_rootDir = _loc->getRoot();
		else
			_rootDir = _serv.getRoot();
		static int nr = 0;
		std::stringstream ss;
		ss << _rootDir << "Download_" << nr;
		_filePath = ss.str();
		nr++;
	}
	else
		locationExists();
	_resp.setCurrentLoc(_loc);
	return (_filePath);
}
