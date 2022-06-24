#ifndef Client_HPP
#define Client_HPP

#include <string>
#include "Server.hpp"

enum Stage
{
	GET_REQUEST,
	END_OF_REQUEST,
	SET_RESOURCE,
	SEND_RESPONSE,
	MAKE_AUTOINDEX,
	CGI_WRITE,
	CGI_READ,
	FILE_WRITE,
	FILE_READ,
	CLOSE,
	WAIT,
};

struct Client
{
    bool keep;
    int origin_fd;
    Stage _stage;
	Server *server;
    std::string request_msg;
    std::string respond_msg;
    std::string tmp_buffer;

    Client() : server(nullptr) {}
    Client(Server *_server) : keep(1), origin_fd(0), _stage(GET_REQUEST), server(_server) {}
};

#endif
