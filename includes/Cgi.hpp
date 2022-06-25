#ifndef CGI_HPP
#define CGI_HPP

#include "Client.hpp"
#include "Connect.hpp"
#include "Config.hpp"
#include "Request_parser.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>

class Cgi
{
	private:

	public:
		const Request&			__request;
		std::vector<std::string> __v_envlist;
		char**					__env;
		char*					__argv[3];
		std::string				__cwd;
		std::string				__requested_uri;
		std::string				__route;
		std::string				__filepath;
		std::string				__query_string;

		Cgi();
		Cgi(Connect connect, int cur_client_fd, Request request);
		// Cgi(const Cgi& other);
		~Cgi();
		// Cgi& operator=(const Cgi& other);

		std::string	m_get_cwd();
		std::string m_get_filepath();
		std::string	m_get_method(int method);
		std::string	m_get_query_string();

		void		m_set_env();
		void		m_set_argv();
		std::string	m_cgi_exec();

		std::string	m_itoa(int n);
		void		m_delete();

#endif