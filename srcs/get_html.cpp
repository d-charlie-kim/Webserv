#include <fstream>
#include <sstream>
#include "../includes/Request_parser.hpp"
#include "../includes/Client.hpp"
#include <iostream>

std::string get_html(std::string request_msg, Server *server)
{
	Request_parser		request(request_msg);

	request.parse_request(server);

	return "";
}
