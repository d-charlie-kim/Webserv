#include <iostream>
#include <vector>
#include <sstream>
#include "../includes/Configure.hpp"
#include "../includes/Server.hpp"
#include "../includes/error.hpp"

void set_conf_location(std::string read_line, std::vector<struct Location> v_location)
{
	// 항목이 존재하면 location으로 없으면 넘어감 (default로 채워져있기 때문)
	// std::cout << "location::: " << read_line << std::endl;

	std::stringstream	iss(read_line);
	std::string			key;
	std::string			value;

	iss >> key;
	if (key == "root")
	{
		iss >> value;
		v_location.back().root = value;
	}
	else if (key == "index")
	{
		while (iss >> value)
		{
			v_location.back().v_index.push_back(value);
		}
	}
	else if (key == "allow_methods")
	{
		while (iss >> value)
		{
			if (value == "GET" && !v_location.back().has_methods_get())
				v_location.back().add_methods_get();
			else if (value == "POST" && !v_location.back().has_methods_post())
				v_location.back().add_methods_post();
			else if (value == "DELETE" && !v_location.back().has_methods_delete())
				v_location.back().add_methods_delete();
			// else
			// 	throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component in Allow_methods");
		}
	}
	else if (key == "error_page")
	{
		int error_num;
		while (iss >> error_num)
			v_location.back().p_error_page.first.push_back(error_num);
		iss >> value;
		v_location.back().p_error_page.second = value;
	}
	else if (key == "client_max_body_size")
	{
		int max_size;
		iss >> max_size;
		v_location.back().client_max_body_size = max_size;		
	}
	else if (key == "auto_index")
	{
		iss >> value;
		if (value == "on")
			v_location.back().auto_index = true;
		else if (value == "off") // default가 off 라서 굳이 안 꺼줘도 되고, off인지만 체크하면 됨
			v_location.back().auto_index = false;
		else
			throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component in auto_index");
	}
	else if (key == "cgi_extension")
	{
		iss >> value;
		v_location.back().cgi = value;
	}
	else
		throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component");

	std::string	eof_check;
	iss >> eof_check;
	if (iss.good())
		throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component value");
}
