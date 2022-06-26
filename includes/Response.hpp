#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

struct Response
{
	std::string	file_path;
	std::string	header;
	std::string body;
};

#endif