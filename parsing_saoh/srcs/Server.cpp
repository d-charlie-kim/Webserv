#include "Server.hpp"

/*
    Location
*/

Location::Location() : methods(0x01), client_max_body_size(256), route("/"), p_return(std::make_pair(0, ""))
{
    root = getcwd(nullptr, 1000);
    root += "/www";
}

Location::~Location() {}

Location::Location(const Location& _l) : auto_index(_l.auto_index), methods(_l.methods), client_max_body_size(_l.client_max_body_size),
    root(_l.root), route(_l.route), v_index(_l.v_index), p_error_page(_l.p_error_page), p_return(_l.p_return), cgi(_l.cgi) {}

void Location::add_methods_get() { methods = methods | 0x01; }
void Location::add_methods_post() { methods = methods | 0x01 << 1; }
void Location::add_methods_delete() { methods = methods | 0x01 << 2; }

bool Location::has_methods_get() { return (methods & 0x01); }
bool Location::has_methods_post() { return (methods & 0x01 << 1); }
bool Location::has_methods_delete() { return (methods & 0x01 << 2); }


/*
    Server
*/

Server::Server() : default_location(), p_listen(std::make_pair("0", 80)) {}