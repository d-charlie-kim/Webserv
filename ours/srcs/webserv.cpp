#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"


/*
	arg check 따로 빼기
*/

static std::string check_arg(int argc, const char *argv[])
{
	if (argc > 3)
		throw std::invalid_argument("too many arguments");
	std::string	path = (argc == 1) ? "./config/our_default.conf" : argv[1];
	size_t		pos = path.rfind('.');
	if (pos == std::string::npos || path.substr(pos) != ".conf")
		throw std::invalid_argument("invalid file extension");
	return path;
}

int main(int argc, const char *argv[])
{
	Config parser;

	try
	{
		std::string path = check_arg(argc, argv);
		parser.open_file(path);
		// parser.print();
		parser.parse_file();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
