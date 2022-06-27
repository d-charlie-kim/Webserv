#include "Server.hpp"
#include "Config.hpp"

Location::Location()
	: auto_index(false),
		methods(0x01),
		client_max_body_size(256),
		p_error_page(std::make_pair(std::vector<int>(1, 404), std::string("/error_page_404.html"))) {}

Location::Location(const struct Location& default_location)
	: auto_index(default_location.auto_index),
		methods(default_location.methods),
		client_max_body_size(default_location.client_max_body_size),
		p_error_page(default_location.p_error_page),
		root(default_location.root),
		route(default_location.route),
		v_index(default_location.v_index),
		p_return(default_location.p_return),
		cgi(default_location.cgi) {}

Location::~Location() {}

Server::Server() : listen(std::make_pair(std::string("*"), 80)) {}

Server::~Server() {}

