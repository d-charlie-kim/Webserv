#include <iostream>
#include <exception>
#include <string>
#include "../includes/Configure.hpp"
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
	Configure	conf;
	conf.v_server_list.push_back("default");
	// std::vector<struct Server> v_server_list;
	// v_server_list.push_back("default");

	try
	{
		if (argc == 1)
			conf.file_path = "../config/default.conf";
		else if (argc == 2 && conf_path_valid_check(argv[1]))
			conf.file_path = argv[1];
		else
			throw ArgumentCountError();
		parsing(conf);
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
