#include <iostream>
#include <exception>
#include <string>
#include "../includes/Parsing.hpp"
#include "../includes/Server.hpp"
#include "../includes/error.hpp"

static bool conf_path_valid_check(const char* str)
{
	std::string tmp(str);
	if(tmp.size() <= 5 || tmp.substr(tmp.size() - 5) != ".conf")
		throw (ArgumentPathError());
	return (true);
}

int main(int argc, char *argv[])
{
	std::vector<struct Server> server_list;
	server_list.push_back("default");

	try
	{
		if (argc == 1)
			parsing("../config/default.conf", server_list);
		else if (argc == 2 && conf_path_valid_check(argv[1]))
			parsing(argv[1], server_list);
		else
			throw ArgumentCountError();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
