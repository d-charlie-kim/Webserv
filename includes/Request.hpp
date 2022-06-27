#ifndef Request_HPP
#define Request_HPP

#include "Server.hpp"

struct Request
{
	int				method;	
	std::string		url; // original whole string
	std::string		path; // url without query
	Location		*location;
	bool			is_cgi;
	bool			is_directory;
	std::string		connection;
	int				content_length;
	std::string		content_type;
	std::string		body;
	int				status_code; // 초기값 0 
};

#endif