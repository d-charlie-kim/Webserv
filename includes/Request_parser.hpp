#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "Server.hpp"

struct Request
{
	int				method;
	std::string		url;
	Location		*location;
	bool			is_valid_version;
	bool			is_cgi;
	bool			is_directory;
	std::string		connection;
	int				content_length;
	std::string		body;
	int				status_code; // 초기값 0 
};

class Request_parser
{
	private:
		std::string							__request_msg;
		std::list<std::string>				__l_line;
		std::list<std::list<std::string> >	__l_file;
		Request								__request;

		Request_parser(const Request_parser &other);
		Request_parser& operator = (const Request_parser &other);
		Request_parser();
		void		split_request_msg();
		Location*	search_location(std::string str, std::vector<Location> &list);
		std::list<std::string> m_next_line();

	public:
		Request_parser(std::string request_msg);
		~Request_parser();

		void		parse_request(Server *server);
		Request		get_request() const;
};

#endif
