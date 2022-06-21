#ifndef Client_HPP
#define Client_HPP

#include <string>
#include "Server.hpp"

struct Client
{
    bool keep;
	Server *server;
    std::string request_msg;
    std::string respond_msg;

    Client() : server(nullptr) {}
    Client(Server *_server) : keep(1), server(_server) {}
};

#endif
