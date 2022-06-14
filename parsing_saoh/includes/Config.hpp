#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "Server.hpp"

class Configure
{		
	public:
		std::vector<Server>			v_server;
		
		Configure(std::string conf_name);
		~Configure();
		void get_conf_stream(std::fstream& fs, const std::string& conf_name);
		void check_and_set_flag_bracket(std::istringstream& iss, bool& server_flag, bool& location_flag, bool& server_bracket, bool& location_bracket);
		void check_and_unset_flag_bracket(std::istringstream& iss, bool& server_flag, bool& location_flag, bool& server_bracket, bool& location_bracket);
		void check_and_set_flag_server(std::istringstream& iss, bool& server_flag);
		void check_and_set_flag_location(std::istringstream& iss, bool& server_flag, bool& location_flag, bool& server_bracket);
		void parsing_listen(std::istringstream& iss);
		void start_parsing(std::fstream& fs);
};

#endif