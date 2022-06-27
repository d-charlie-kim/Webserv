#ifndef CGI_HPP
#define CGI_HPP

#include "Connect.hpp"
#include "Client.hpp"
#include <string>
#include <vector>

class Cgi
{
	public:
		int						m_cgi_exec();
		Cgi(Connect& connect, Client& client);
		~Cgi();

	private:
		Connect&				__cn;
		const Request&			__request;
		std::vector<std::string> __v_envlist;
		char**					__env;
		char*					__argv[3];
		std::string				__cwd;
		std::string				__requested_uri;
		std::string				__route;
		std::string				__filepath;
		std::string				__query_string;

		std::string				m_get_cwd();
		std::string 			m_get_filepath();
		std::string				m_get_method(int method);
		std::string				m_get_query_string();

		void					m_set_env();
		void					m_set_argv();
		void					m_delete();
};

static std::string	ft_itoa(int n);

#endif