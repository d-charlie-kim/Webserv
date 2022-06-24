#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Cgi
{
	private:

	public:
		std::vector<std::string> __v_envlist;
		// request info 담고있는 클래스 추가
		// std::string				qs;
		char**					__env;
		char*					__argv[3];
		std::string				__cwd;
		std::string				__requested_uri;
		std::string				__route;

		Cgi();
		// Cgi(Connect connect, int cur_client_fd, Request request);
		// Cgi(const Cgi& other);
		~Cgi();
		// Cgi& operator=(const Cgi& other);

		std::string	m_cgi_exec();
		void		m_get_cwd();
		void		m_set_env();
		void		m_set_argv();
};

#endif