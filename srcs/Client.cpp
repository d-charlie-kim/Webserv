#include "Client.hpp"

Client::Client() : server(nullptr) {}

Client::Client(Server *_server) : keep(1), is_io(0), origin_fd(0), cgi_pid(0), _stage(GET_REQUEST), server(_server) {}

void Client::client_clear()
{
	request_msg.clear();
    respond_msg.clear();
    tmp_buffer.clear();
    keep = 1;
	is_io = 0;
    origin_fd = 0;
    cgi_pid = 0;
    rs.clear();
}