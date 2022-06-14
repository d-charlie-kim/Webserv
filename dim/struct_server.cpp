#include "struct_server.hpp"

#include <arpa/inet.h>

Server::Server()
: p_listen("INADDR_ANY", 80), server_name("") {std::cout << "server default constructor" << std::endl;}

Server::~Server() {}

Server::Server(const Server& other)
{
	p_listen = other.p_listen;
	server_name = other.server_name;
	default_location = other.default_location;
	v_location = other.v_location;
	std::cout << "server copy constructor" << std::endl;
}

// 질문 default value : root, route, v_index, p_error_page, cgi ??????
Location::Location()
: auto_index(false), methods(0x01), client_max_body_size(256)
{
	p_error_page.first.push_back(404); // defalt 아닌 location에서 fritst-vector 값 덮어씌우는거 추가해야됨
	p_error_page.second = "/error_page_404.html";
	std::cout << "loc default constructor" << std::endl;
}

Location::~Location() {}

Location::Location(const Location& other)
{
	auto_index = other.auto_index;
	methods = other.methods;
	client_max_body_size = other.client_max_body_size;
	root = other.root;
	route = other.route;
	v_index = other.v_index;
	p_error_page = other.p_error_page;
	p_return = other.p_return;
	cgi = other.cgi;
	std::cout << "loc copy constructor" << std::endl;
}

void Location::add_methods_get() { methods = methods | 0x01; }
void Location::add_methods_post() { methods = methods | 0x01 << 1; }
void Location::add_methods_delete() { methods = methods | 0x01 << 2; }
bool Location::has_methods_get() { return (methods & 0x01); }
bool Location::has_methods_post() { return (methods & 0x01 << 1); }
bool Location::has_methods_delete() { return (methods & 0x01 << 2); }

