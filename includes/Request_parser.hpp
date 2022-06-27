#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Client.hpp"

class Request_parser
{
	private:
		std::string							__request_msg;
		std::list<std::string>				__l_line;
		std::list<std::list<std::string> >	__l_file;

		Request_parser(const Request_parser &other);
		Request_parser& operator = (const Request_parser &other);
		Request_parser();
		void		split_request_msg();
		Location*	search_location(std::string str, std::vector<Location> &list);
		std::list<std::string> m_next_line();

	public:
		Request_parser(std::string request_msg);
		~Request_parser();
		Request								request;

		void		parse_request(Client& client);
};

#endif
