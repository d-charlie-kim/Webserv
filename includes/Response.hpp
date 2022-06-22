#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>

struct Request 
{
	int				method;
	std::string		url;
	// Location		*location;
	bool			is_valid_version;
	bool			is_cgi;
	bool			is_directory;
	std::string		connection;
	int				content_length;
	std::string		body;
};

struct Response
{
	int			status_code = 0;
	std::string	file_path;
	std::string	header;
	std::string body;
};

#endif
