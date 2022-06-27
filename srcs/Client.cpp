#include "Client.hpp"

Client::Client() : server(nullptr) {}

Client::Client(Server *_server) : keep(1), is_io(0), origin_fd(0), cgi_pid(0), _stage(GET_REQUEST), server(_server) {}

void Client::client_clear()
{
	cn.clients[cn.curr_event->ident].request_msg.clear();
    cn.clients[cn.curr_event->ident].respond_msg.clear();
    cn.clients[cn.curr_event->ident].tmp_buffer.clear();
    cn.clients[cn.curr_event->ident].keep = 1;
	cn.clients[cn.curr_event->ident].is_io = 0;
    cn.clients[cn.curr_event->ident].origin_fd = 0;
    cn.clients[cn.curr_event->ident].cgi_pid = 0;
    cn.clients[cn.curr_event->ident].rs.clear();
}