#ifndef Server_HPP
#define Server_HPP

#include <vector>
#include <string>
#include <unistd.h>
#include <stdexcept>

struct Location
{
	bool						auto_index;
	int							methods;
	int							client_max_body_size;
	std::string					root;
	std::string					route;
	std::vector<std::string>	v_index;
	std::pair<std::vector<int>, std::string>	p_error_page;
	std::pair<int, std::string>	p_return;
	std::string					cgi;

	Location();
	~Location();
	Location(const Location& _l);
	void add_methods_get();
	void add_methods_post();
	void add_methods_delete();
	bool has_methods_get();
	bool has_methods_post();
	bool has_methods_delete();
};

struct Server
{
	std::string						server_name;
	struct Location					default_location;
	std::pair<std::string, int>		p_listen;
	std::vector<struct Location>	v_location;
	Server();
	~Server();
};

#endif