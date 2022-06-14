#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>

#include "struct_server.hpp"

#define SERVER 0
#define LOCATION 1

class Configure
{
	private:

	public:
		Configure();
		Configure(std::string name);
		~Configure();
		Configure& operator=(const Configure& other);

		std::string			__name;
		std::stack<int>		__s_block_pos;
		std::vector<Server>	__v_server;
		std::string			__line;
		int					__server_size;
		int					__location_size;

		void			m_parsing();
		void			m_check_file_name();
		void			m_open_file(std::ifstream& input_stream);
		void			m_pop_block();
		void			m_push_block();
		std::string		m_trim(size_t i);
		bool			m_is_empty_after(size_t i);
		void			m_directive_parsing();
		void			m_server_directive();
		void			m_location_directive();
		void			m_matching_server_token(std::istringstream& iss);
		void			m_matching_location_token(std::istringstream& iss);
		void			m_add_server_name(std::istringstream& iss);
		void			m_add_listen(std::istringstream& iss);
		void			m_add_root(std::istringstream& iss, Location& location);
		void			m_add_index(std::istringstream& iss, Location& location);
		void			m_add_allow_methods(std::istringstream& iss, Location& location);
		void			m_add_error_page(std::istringstream& iss, Location& location);
		void			m_add_client_max_body_size(std::istringstream& iss, Location& location);
		void			m_add_auto_index(std::istringstream& iss, Location& location);
		void			m_add_return(std::istringstream& iss, Location& location);
		void			m_add_cgi_extension(std::istringstream& iss, Location& location);

};

#endif

