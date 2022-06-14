#include <iostream>
#include <exception>
#include <string>
#include "../includes/Configure.hpp"
#include "../includes/Server.hpp"
#include "../includes/error.hpp"


// This is test code printing the Configure File to check parsing is done well
/*
#include <vector>
void print_conf(struct Configure conf)
{
	std::cout << "---- [ TEST START ] ----\n";
	std::cout << "\n Numbers of Server : " << conf.v_server_list.size() << std::endl;
	std::cout << std::endl;

	// int	count = 1;
	// for (std::vector<int>::iterator iter = conf.v_server_list.begin(); iter != conf.v_server_list.end(); iter++)
	// {
	// 	std::cout << "   <<  No." << count << " Server  >>\n";
		
	// }
}
*/

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

	try
	{
		if (argc == 1)
			conf.file_path = "../config/simple.conf";
		else if (argc == 2 && conf_path_valid_check(argv[1]))
			conf.file_path = argv[1];
		else
			throw ArgumentCountError();
		parsing(conf);
		// print_conf(conf);
		// validate(conf.v_server_list);
		// execute(conf.v_server_list);
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
