#ifndef Client_HPP
#define Client_HPP

#include <string>
#include "Server.hpp"

struct Client
{
	Server *server;
    std::string request_msg;
    std::string respond_msg;

    Client() : server(nullptr) {}
    Client(Server *_server) : server(_server) {}
};

#endif
