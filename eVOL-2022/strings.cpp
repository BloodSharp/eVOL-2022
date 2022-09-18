#include "client.h"

std::string getfilename(std::string path)
{
	path = path.substr(path.find_last_of("/\\") + 1);
	size_t dot_i = path.find_last_of('.');
	return path.substr(0, dot_i);
}

std::string strip(std::string path)
{
	size_t dot = path.find("/models/");
	path.erase(path.begin(), path.begin() + dot + 1);
	return path;
}
