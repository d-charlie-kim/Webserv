#ifndef Client_HPP
#define Client_HPP

#include <string>
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

enum Stage
{
	GET_REQUEST,
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
	bool is_io;
    int origin_fd;
	pid_t cgi_pid;
    Stage _stage;
	Server *server;
	Request rq;
	Response rs;
    std::string request_msg;
    std::string respond_msg;
    std::string tmp_buffer;

    Client() : server(nullptr) {}
    Client(Server *_server) : keep(1), is_io(0), origin_fd(0), cgi_pid(0), _stage(GET_REQUEST), server(_server) {}
};

#endif
