#include <string>
#include <sys/stat.h>
#include "../Server/location.hpp"
#include "../Server/server.hpp"
#include "../Request/request.hpp"
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

	uri = req.getUri();
	location *loc = findFileExtension(serv, &uri);
	found = uri.find_first_of("/", 1);
	if (found == std::string::npos)
		found = 1;
	locMatch = uri.substr(0, found);
	if (uri.length() > 1)
		uri.erase(0, 1);
	if (!loc)
		loc = serv.findLocation(locMatch);
	if (!loc)
		resp.setStatus(404); // location not found
	else
	{
		resp.currentLoc = loc;
		// location exists
		if (!loc->getRoot().empty()) // location has no own root, so we use the server root
			rootDir = loc->getRoot();
		else
			rootDir = serv.getRoot();

		std::vector<std::string>	indices;
		if (uri[uri.length() - 1] == '/') // uri ends in '/', so a directory is requested, meaning we have to fetch an index page
		{
			if (!loc->getIndices().empty())
				indices = loc->getIndices(); // if location has no index specifications, we use the server's
			else
				indices = serv.getIndices();
			std::vector<std::string>::iterator it;
			for (it = indices.begin(); it < indices.end(); it++) // test from front to back to find the first existing index page at requested root
			{
				filePath = rootDir + (*it);
				struct stat statBuf;
				if (stat(filePath.c_str(), &statBuf) == 0)
					break;
			}
			if (it == indices.end()) // all index pages don't exist at requested root
				resp.setStatus(404);
		}
		else // uri does not end in '/', so a specific file is requested
		{
			filePath = rootDir + uri;
			struct stat statBuf;
			if (stat(filePath.c_str(), &statBuf) != 0)
				resp.setStatus(404);
		}
	}
	return (filePath);
}
