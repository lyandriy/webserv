#include "webserv.hpp"
#include <fstream>

int conf_file(std::string file)
{
    std::ifstream	in_file(file.c_str());
    std::string		line;

    if (in_file.is_open())
	{
		std::getline(in_file, line);
    }
}