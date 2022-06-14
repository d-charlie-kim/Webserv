#ifndef STRUCT_SERVER_HPP
#define STRUCT_SERVER_HPP

#include <iostream>
#include <string>
#include <vector>

struct Location
{
	bool										auto_index;
	int											methods;
	int											client_max_body_size;
	std::string									root;
	std::string									route;
	std::vector<std::string>					v_index;
	std::pair<std::vector<int>, std::string>	p_error_page;
	std::pair<int, std::string>					p_return;
	std::string									cgi;

	Location();
	// Location(std::string name);
	~Location();
	Location(const Location& default_location);
	void add_methods_get();
	void add_methods_post();
	void add_methods_delete();
	bool has_methods_get();
	bool has_methods_post();
	bool has_methods_delete();
};

struct	Server
{
	std::pair<std::string, int>		p_listen;
	std::string						server_name; //인자 한개만 받는지 확인하기
	struct Location					default_location;
	std::vector<struct Location>	v_location;

	Server();
	// Server(std::string name);
	~Server();
	Server(const Server& default_server);
};

#endif
