#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>

struct Location
{
	bool						auto_index;
	int							methods;
	int							client_max_body_size;
	std::string					root;
	std::string					route; //path
	std::vector<std::string>	v_index;
	std::pair<std::vector<int>, std::string>	p_error_page;
	std::pair<int, std::string>	p_return; //없을때 int값 0
	std::string					cgi;

	Location();
	~Location();
	Location(const struct Location& default_location);

	void add_methods_get();
	void add_methods_post();
	void add_methods_delete();

	bool has_methods_get();
	bool has_methods_post();
	bool has_methods_delete();
};

struct Server
{
	std::pair<std::string, int>		p_listen;
	std::string						server_name;
	struct Location					default_location;
	std::vector<struct Location>	v_location;

	Server(const char* default_name);
	~Server();
};

#endif