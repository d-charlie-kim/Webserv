#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>
#include <list>
#include <stack>

class Config
{
	private:
		std::list<std::list<std::string> >	__file;
		std::stack<std::string>				__brace;
		std::vector<Server>					__server_list;
		Config(const Config &other);
		Config& operator = ( const Config &a );

		std::list<std::string>	m_next_line(int brace_check);
		std::list<std::string>	m_split_line(std::string str);
		void					m_parse_server(std::list<std::string> &line);
		Location				m_parse_location(std::list<std::string> &line, Location loc);
		void		m_is_valid_error_code(int code);

		void					m_parse_listen(Server &new_server, std::list<std::string> &line);
		void					m_parse_server_name(Server &new_server, std::list<std::string> &line);
		void					m_parse_root(Server &new_server, std::list<std::string> &line);
		void					m_parse_index(std::list<std::string> &line, Location &loc);
		void					m_parse_allow_methods(std::list<std::string> &line, Location &loc);
		void					m_parse_error_page(std::list<std::string> &line, Location &loc);
		void					m_parse_clent_max_body_size(std::list<std::string> &line, Location &loc);
		void					m_parse_auto_index(std::list<std::string> &line, Location &loc);
		void					m_parse_cgi_extension(std::list<std::string> &line, Location &loc);
		void					m_parse_return(std::list<std::string> &line, Location &loc);


	public:
		Config();
		~Config();
		void					open_file(std::string &file_path);
		void					parse_file();
		std::vector<Server>		get_server_list() const;
};

#endif
