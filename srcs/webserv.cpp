#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

/*
void	print_location_list(Location &loc)
{
	std::cout << "\tlocation path : " << loc.route << std::endl;
	std::cout << "\tlocation root : " << loc.root << std::endl;
	std::cout << "\tindex : ";
	for(std::vector<std::string>::iterator it = loc.v_index.begin(); it != loc.v_index.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "\tallow_method : ";
	const char *method[3] = {"GET", "POST", "DELETE"};
	int	bit = 0x01;
	for (int i = 0; i < 3; i++)
	{
		if (loc.methods & bit)
			std::cout << method[i] << " ";
		bit <<= 1;
	}
	std::cout << std::endl;
	std::cout << "\terror_page : ";
	for(std::vector<int>::iterator it = loc.p_error_page.first.begin(); it != loc.p_error_page.first.end(); it++)
		std::cout << *it << " ";

	std::cout << loc.p_error_page.second << std::endl;
	std::cout << "\tclient_max : " << loc.client_max_body_size  << std::endl;
	std::cout << "\treturn : " << loc.p_return.first << " " << loc.p_return.second << std::endl;
	std::cout << "\tcgi : " << loc.cgi << std::endl;
	std::cout << "\tauto_index : " << loc.auto_index << std::endl;
}

void	print_server_list(std::vector<Server> &list)
{
	for (std::vector<Server>::iterator it = list.begin(); it != list.end(); it++)
	{
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "server_listen ip :" << it->listen.first << std::endl;
		std::cout << "server_listen port :" << it->listen.second << std::endl;
		std::cout << "server_name : " << it->server_name << std::endl;
		std::cout << "default location" << std::endl;
		print_location_list(it->default_location);
		for(std::vector<Location>::iterator loc=it->location.begin(); loc != it->location.end(); loc++)
		{
			std::cout << "-----------------------------------" << std::endl;
			std::cout << "location" << std::endl;
			print_location_list(*loc);
		}
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "-----------------------------------" << std::endl;

	}
}
*/

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
	exec_server(parser.get_server_list());


	// std::vector<Server> server_list = parser.get_server_list();
	// print_server_list(server_list);

	return (0);
}
