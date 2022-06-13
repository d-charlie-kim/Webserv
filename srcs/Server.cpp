#include "../includes/Server.hpp"

Location::Location()
: auto_index(false),
methods(0),
client_max_body_size(256),
root(),
route(),
v_index(),
p_error_page(),
p_return(),
cgi()
{
	add_methods_get();
	std::cout << "Default Location Created\n";
}

Location::~Location() {
	std::cout << "Location Deleted\n";
}

Location::Location(const struct Location& default_location)
: auto_index(default_location.auto_index),
methods(default_location.methods),
client_max_body_size(default_location.client_max_body_size),
root(default_location.root),
route(default_location.route),
v_index(default_location.v_index),
p_error_page(default_location.p_error_page),
p_return(default_location.p_return),
cgi(default_location.cgi)
{
	std::cout << "Location Created by Copying Default Location\n";
}

void Location::add_methods_get() { methods = methods | 0x01; }
void Location::add_methods_post() { methods = methods | 0x01 << 1; }
void Location::add_methods_delete() { methods = methods | 0x01 << 2; }

bool Location::has_methods_get() { return (methods & 0x01); }
bool Location::has_methods_post() { return (methods & 0x01 << 1); }
bool Location::has_methods_delete() { return (methods & 0x01 << 2); }

Server::Server(const char* default_name)
: p_listen("127.0.0.1", 80)
{
	std::string temp(default_name);
	server_name = temp;
	std::cout << "Server List Created\n";
}

Server::~Server()
{
	std::cout << "[ " << server_name << " ] Server Deleted\n";
}

