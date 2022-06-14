#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../includes/Configure.hpp"
#include "../includes/Server.hpp"
#include "../includes/error.hpp"

void set_conf_server(std::string read_line, std::vector<struct Server> v_server_list)
{
	// 여기 들어오는 것들은 server 구조체 혹은 default location 으로 간다.
	// std::cout << "server::: " << read_line << std::endl;

	std::stringstream	iss(read_line);
	std::string			key;
	std::string			value;

	iss >> key;
	if (key == "listen")
	{
		iss >> value;
		v_server_list.back().p_listen.first = value.substr(0, value.find(":"));
		v_server_list.back().p_listen.second = atoi(value.substr(0, value.find(":")).c_str());
		// atoi 오버플로우 나는 경우를 체크해야 하는가.
		// 오버플로우 나서 양수가 되는 경우?
	}
	else if (key == "server_name")
	{
		iss >> value;
		v_server_list.back().server_name = value;
	}
	else if (key == "root")
	{
		iss >> value;
		v_server_list.back().default_location.root = value;
	}
	else if (key == "index")
	{
		while (iss >> value)
		{
			v_server_list.back().default_location.v_index.push_back(value);
		}
	}
	else if (key == "allow_methods")
	{
		// 다 잘 받아주지만, GET GET DELETE 처럼 같은 method가 여러번 나올 경우는 걸러주지 못함.
		// 해주려면 if 문에 has 까지 체크해야 함
		// 중복되어도 유효하다면, 해당 부분만 지워주면 됨

		// default 가 GET이라서 GET은 이미 들고 있을텐데, location에 DELETE POST만 주어진다면 0으로 만들어줘야해서
		// delete_methods_get() 도 필요할듯.

		// 아니면 default를 0으로 두고, 파싱 한번 후에 여전히 0이면 GET만 켜주는 방식도 괜찮아보임
		while (iss >> value)
		{
			if (value == "GET" && !v_server_list.back().default_location.has_methods_get())
				v_server_list.back().default_location.add_methods_get();
			else if (value == "POST" && !v_server_list.back().default_location.has_methods_post())
				v_server_list.back().default_location.add_methods_post();
			else if (value == "DELETE" && !v_server_list.back().default_location.has_methods_delete())
				v_server_list.back().default_location.add_methods_delete();
			// else
			// 	throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component in Allow_methods");
		}
	}
	else if (key == "error_page")
	{
		int error_num;
		while (iss >> error_num)
			v_server_list.back().default_location.p_error_page.first.push_back(error_num);
		iss >> value;
		v_server_list.back().default_location.p_error_page.second = value;
	}
	else if (key == "client_max_body_size")
	{
		int max_size;
		iss >> max_size;
		v_server_list.back().default_location.client_max_body_size = max_size;		
	}
	else if (key == "auto_index")
	{
		iss >> value;
		if (value == "on")
			v_server_list.back().default_location.auto_index = true;
		else if (value == "off") // default가 off 라서 굳이 안 꺼줘도 되고, off인지만 체크하면 됨
			v_server_list.back().default_location.auto_index = false;
		else
			throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component in auto_index");
	}
	else
		throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component");

	std::string	eof_check;
	iss >> eof_check;
	if (iss.good())
		throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component value");
}
