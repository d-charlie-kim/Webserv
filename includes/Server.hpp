#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "Client.hpp"

#define GET 0x01
#define POST 0x02
#define DELETE 0x04 

struct Location
{
	private:
		// Location& Location::operator=(const )
	public:
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

void exec_server(std::vector<Server>& server_list);
void get_html(Client &client);

#endif
