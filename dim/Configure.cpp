#include "Configure.hpp"
#include "error.hpp"
#include <sstream>

Configure::Configure()
: __name("./default.conf") { std::cout << "conf default constructor" << std::endl;}

Configure::Configure(std::string name)
: __name(name) { std::cout << "conf constructor with name" << std::endl;}

Configure::~Configure() {}

Configure& Configure::operator=(const Configure& other)
{
	__name = other.__name;
	__s_block_pos = other.__s_block_pos;
	__v_server = other.__v_server;
	__line = other.__line;
	std::cout << "conf assignment operator" << std::endl;
	// etc...
	return (*this);
}

void	Configure::m_check_file_name()
{
	if (__name.size() < 5)
		throw (arg_error());
	if (__name.find(".conf", __name.size() - 5) != std::string::npos)
		throw (file_name_error());
}

void	Configure::m_open_file(std::ifstream& input_stream)
{
	input_stream.open(__name);
	if (input_stream.is_open() == false)
		throw(file_open_error());
}

bool	Configure::m_is_empty_after(size_t i)
{
	if (__line.size() > i && __line.find_first_not_of(" \t", i) != std::string::npos)
		return (false);
	return (true);
}

void	Configure::m_pop_block()
{
	if (__s_block_pos.size() == 0)
		throw (config_form_error());
	__s_block_pos.pop();
}

std::string	Configure::m_trim(size_t i)
{
	size_t start = __line.find_first_not_of(" \t", i);
	if (start == std::string::npos)
		throw (config_form_error());
	size_t end = __line.find_last_not_of(" \t");
	return (__line.substr(start, end - start + 1));
}

void	Configure::m_push_block()
{
	if (__line.find("server") == 0)
	{
		if (m_is_empty_after(6) == false || (__s_block_pos.size() != 0 && __s_block_pos.top() == SERVER))
			throw(config_form_error());
		__s_block_pos.push(SERVER);
		__v_server.push_back(Server());
		__server_size++;
	}
	if (__line.find("location") == 0)
	{
		if (__s_block_pos.size() != 0 && __s_block_pos.top() != SERVER)
			throw(config_form_error());
		__s_block_pos.push(LOCATION);
		__v_server[__v_server.size() - 1].v_location.push_back(Location());
		__location_size++;
		__v_server[__v_server.size() - 1].v_location[__location_size - 1].route = m_trim(8);
	}
}

void	Configure::m_add_server_name(std::istringstream& iss)
{
	std::string	token;
	iss >> __v_server[__server_size - 1].server_name;
}

void	Configure::m_add_listen(std::istringstream& iss)
{
	std::string	token;
	if (token.find(":") == std::string::npos)
	{
		if (token.find(".") == std::string::npos)
			__v_server[__server_size - 1].p_listen.second = std::stoi(token); // 허용함수아님
		else
			__v_server[__server_size - 1].p_listen.first = token;
	}
	else
	{
		// ':' 기준으로 나눠서 파싱 추가
	}
	if (!iss.eof())
	{
		if (token.find(":") == std::string::npos)
		{
			if (token.find(".") == std::string::npos)
				__v_server[__server_size - 1].p_listen.second = std::stoi(token); // 허용함수아님
			else
				__v_server[__server_size - 1].p_listen.first = token;
		}
	}
}

void	Configure::m_add_root(std::istringstream& iss, Location& location)
{
	iss >> location.root;
}

void	Configure::m_add_index(std::istringstream& iss, Location& location)
{
	std::string	token;
	while (iss >> token)
		location.v_index.push_back(token);
}

void	Configure::m_add_allow_methods(std::istringstream& iss, Location& location)
{
	std::string	token;
	while (iss >> token)
	{
		if (token == "GET") { location.add_methods_get(); }
		if (token == "POST") { location.add_methods_post(); }
		if (token == "DELETE") { location.add_methods_delete(); }
		else
			throw(config_form_error());
	}
}

void	Configure::m_add_error_page(std::istringstream& iss, Location& location)
{
	std::string token;
	while (iss >> token)
	{
		if (token.find(".") == std::string::npos)
			location.p_error_page.first.push_back(std::stoi(token));
		else
			location.p_error_page.second = token;
	}
}

void	Configure::m_add_client_max_body_size(std::istringstream& iss, Location& location)
{
	int size;
	iss >> size;
	location.client_max_body_size = size;
	if (!iss.eof())
		throw (config_form_error());
}

void	Configure::m_add_auto_index(std::istringstream& iss, Location& location)
{
	std::string token;
	iss >> token;
	if (token != "on" && token != "off")
		throw (config_form_error());
	if (token == "on")
		location.auto_index = true;
}

void	Configure::m_add_return(std::istringstream& iss, Location& location)
{
	(void)iss;
	(void)location;
	// std::string token;
	// while (iss >> token)
	// {
	// 	if (token.find(".") == std::string::npos)
	// 		location.p_return.first = std::stoi(token);
	// 	else
	// 		location.p_return_page.second = token;
	// }
}

void	Configure::m_add_cgi_extension(std::istringstream& iss, Location& location)
{
	(void)iss;
	(void)location;
}

void	Configure::m_server_directive()
{
	std::istringstream	iss(__line);
	std::string			token;

	while (iss >> token)
	{
		if (token == "server_name")	{ m_add_server_name(iss); }
		else if (token == "listen") { m_add_listen(iss); }
		else if (token == "root") { m_add_root(iss, __v_server[__server_size - 1].default_location); }
		else if (token == "index") { m_add_index(iss, __v_server[__server_size - 1].default_location); }
		else if (token == "allow_methods") { m_add_allow_methods(iss, __v_server[__server_size - 1].default_location); }
		else if (token == "error_page") { m_add_error_page(iss, __v_server[__server_size - 1].default_location); }
		else if (token == "client_max_body_size") { m_add_client_max_body_size(iss, __v_server[__server_size - 1].default_location); }
		else if (token == "auto_index") { m_add_auto_index(iss, __v_server[__server_size - 1].default_location); }
		else
			throw(config_form_error());
	}

}

void	Configure::m_location_directive()
{
	std::istringstream	iss(__line);
	std::string			token;

	while (iss >> token) // iss가 eof인 상태에서 또 오면 ...? !iss.eof() 추가
	{
		if (token == "root") { m_add_root(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "index") { m_add_index(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "allow_methods") { m_add_allow_methods(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "error_page") { m_add_error_page(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "client_max_body_size") { m_add_client_max_body_size(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "auto_index") { m_add_auto_index(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "return") { m_add_return(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else if (token == "cgi_extension") { m_add_cgi_extension(iss, __v_server[__server_size - 1].v_location[__location_size - 1]); }
		else
			throw(config_form_error());
	}
}

void	Configure::m_directive_parsing()
{
	if (__line == "\0")
		return ;
	if (__s_block_pos.size() == 0)
		throw (config_form_error());
	if (__s_block_pos.top() == SERVER)
		m_server_directive();
	else if (__s_block_pos.top() == LOCATION)
		m_location_directive();
	else
		throw(config_form_error());
}

void	Configure::m_parsing()
{
	std::ifstream	input_stream;

	m_open_file(input_stream);
	while (getline(input_stream >> std::ws, __line))
	{
		if (__line[0] == '#')
			continue;
		else if (__line[0] == '}' && m_is_empty_after(1))
			m_pop_block();
		else if ((__line.find("server") == 0 && __line[6] != '_') || __line.find("location") == 0)
		{
			m_push_block();
			getline(input_stream >> std::ws, __line);
			if (__line[0] != '{' || m_is_empty_after(1) == false)
				throw(config_form_error());
		}
		else
			m_directive_parsing();
	}
	input_stream.close();
}
