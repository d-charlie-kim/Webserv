#include <sys/socket.h>
#include <netinet/in.h>
#include "../includes/Config.hpp"

Location::Location() : auto_index(false), methods(0x01), client_max_body_size(256), p_error_page(std::make_pair(std::vector<int>(1, 404), std::string("/error_page_404.html"))) { }
Location::~Location() { }

Server::Server() : listen(std::make_pair(std::string("*"), 80)){ }

Server::~Server()
{ }

