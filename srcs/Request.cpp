#include "Request.hpp"

Request::Request() : method(0), url(""), path(""), location(nullptr), is_cgi(0), is_directory(0),
					connection(""), content_length(0), content_type(""), body(""), status_code(0) {}

Request& Request::operator=(Request& rq)
{
	method = rq.method;
	url = rq.body;
	path = rq.path;
	location = rq.location;
	is_cgi = rq.is_cgi;
	is_directory = rq.is_directory;
	connection = rq.connection;
	content_length = rq.content_length;
	content_type = rq.content_type;
	body = rq.body;
	status_code = rq.status_code;
	return (*this);
}