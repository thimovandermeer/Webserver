#include "Response.hpp"

location*	findFileExtension(server &server, std::string *uri)
{
	std::vector<location*> locs = server.getLocations();

	for (std::vector<location*>::iterator it = locs.begin(); it < locs.end(); it++)
	{
		if ((*it)->isFileExtension())
		{
			std::string	extension = (*it)->getMatch();
			if (extension == "*.error_image.png") // some hardcoding for our error img...
				extension.erase(0, 2);
			else
				extension.erase(0, 1);
			size_t len = extension.length();
			if (uri->length() >= len && !uri->compare(uri->length() - len, len, extension))
			{
				if (extension == "error_image.png") // some more hardcoding...
					*uri = "/error_image.png";
				return (*it);
			}
		}
	}
	return (NULL);
}

std::string	getPath(server &serv, Request &req, Response &resp)
{
	std::string filePath;
	std::string rootDir;
	std::string uri;
	std::string locMatch;
	size_t		found;
	location	*loc = NULL;
	bool		needIndex = false;

	uri = req.getUri();
	loc = findFileExtension(serv, &uri);
	if (!loc)
	{
		if (uri.find('.') != std::string::npos) // file requested
		{
			found = uri.find_first_of('/', 1);
			if (uri.find_last_of('/') == 0)
			{
				locMatch = "/";
				uri.erase(0, 1);
			}
			else
				locMatch = uri.substr(0, found);
			if (uri.length() > 1 && locMatch != "/")
			{
				if (found != std::string::npos)
					uri.erase(0, found + 1);
				else
					uri.erase(0, found);
			}
		}
		else // location index requested
		{
			needIndex = true;
			if (uri == "/")
				locMatch = "/";
			else
			{
				found = uri.find_first_of('/', 1);
				locMatch = uri.substr(0, found);
				if (found != std::string::npos)
					uri.erase(0, found + 1);
				else
					uri.erase(0, found);
				if (uri.length() && uri[uri.length() - 1] != '/' && uri != "/") // add '/' at end
					uri += "/";
			}

		}
		loc = serv.findLocation(locMatch);
	}
	else
		uri.erase(0, 1);
	if (!loc && req.getMethod().compare("PUT") != 0)
		resp.setStatus(404); // location not found
	else
	{
		// location exists
		struct stat statBuf = {};
		if (!loc->getRoot().empty()) // location has no own root, so we use the server root
			rootDir = loc->getRoot();
		else
			rootDir = serv.getRoot();

		if (!loc->getCgiPath().empty()) // cgi regel die we gister bedacht hadden?
		{
			filePath = rootDir + loc->getCgiPath();
			resp.currentLoc = loc;
			return (filePath);
		}
		if (needIndex && req.getMethod().compare("PUT") != 0)
		{
			std::vector<std::string>	indices;

			if (!loc->getIndices().empty())
				indices = loc->getIndices();
			else
				indices = serv.getIndices();

			std::vector<std::string>::iterator it; // if empty, it will never loop and (it == indices.end()) will be true
			for (it = indices.begin(); it < indices.end(); it++) // test from front to back to find the first existing index page at requested root
			{
				filePath = rootDir + uri + (*it);
				if (stat(filePath.c_str(), &statBuf) == 0)
					break;
			}
			if (it == indices.end()) // all index pages don't exist at requested root
				resp.setStatus(404);
			// if 404 and loc.getAutoindex == true, do the autoindex thing
		}
		else
		{
			filePath = rootDir + uri;
			if (stat(filePath.c_str(), &statBuf) != 0 && req.getMethod().compare("PUT") != 0)
				resp.setStatus(404);
		}
	}
	resp.currentLoc = loc;
	return (filePath);
}
