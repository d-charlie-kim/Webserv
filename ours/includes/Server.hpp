#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <iostream>
#include <vector>
#include <stack>

/*
	복사 생성자 추가
*/

struct Location
{
	bool						auto_index;
	int							methods;
	int							client_max_body_size;
	std::pair<std::vector<int>, std::string>	p_error_page;
	std::string					root;
	std::string					route;
	std::vector<std::string>	v_index;
	std::pair<int, std::string>	p_return;
	std::string					cgi;

	Location();
	~Location();
	Location(const Location& default_location);
};

struct Server
{
	std::pair<std::string, int>		listen;
	std::string						server_name;
	struct Location					default_location;
	std::vector<struct Location>	location;

	Server();
	~Server();
};

#endif
