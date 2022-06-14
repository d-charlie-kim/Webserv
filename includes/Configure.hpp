#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP

#include <iostream>
#include <vector>
#include <stack>
#include "../includes/Server.hpp"

enum conf {SERVER = 1, LOCATION};

struct Configure
{
	bool						server_flag;
	bool						location_flag;
	std::string					file_path;
	std::stack<int>				s_brace;
	std::vector<struct Server>	v_server_list;
};

void parsing(struct Configure& conf);
void set_conf_location(std::string read_line, std::vector<struct Location> v_location);
void set_conf_server(std::string read_line, std::vector<struct Server> v_server_list);

#endif
