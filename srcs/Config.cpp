#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

Config::Config() { }
Config::~Config() { }

std::vector<Server>& Config::get_server_list() { return __v_server_list; }

std::list<std::string> Config::m_next_line(int brace_check = 0)
{
	std::list<std::string> line;

	while (brace_check >= 0)
	{
		if (__l_file.size() > 1)
			__l_file.pop_front();
		line = __l_file.front();
		if (brace_check && line.front() != "{" && line.size() != 1)
			throw std::invalid_argument("invalid config file");
		brace_check--;
	}
	return (line);
}

void		Config::m_is_valid_error_code(int code)
{
	if (code < 300)
		throw std::invalid_argument("invalid config file");
}


void		Config::m_parse_listen(Server& new_server, std::list<std::string>& line)
{
	if (line.size() != 2)
		throw std::invalid_argument("invalid config file");
	line.pop_front();
	std::string str = line.front();
	size_t pos = str.rfind(':');
	if (pos != std::string::npos)
	{
		int dot_count = 0;
		for (size_t i = 0; i < pos; i++)
			if (str[i] == '.')
				dot_count++;
		if (dot_count != 3)
			throw std::invalid_argument("invalid config file");
		new_server.listen.first = str.substr(0, pos);
		str = str.substr(pos + 1);
	}
	std::stringstream iss(str);
	int port;
	iss >> port;
	if (iss.fail() || !iss.eof() || port < 0 || port > 49151) // atoi 로는 에러가 나서 0이 return 된건지, 원래 값이 0인건지 구분할 수 없음. 그래서 stringstream 사용
		throw std::invalid_argument("invalid config file");
	new_server.listen.second = port;
	line = m_next_line(0);
}

void		Config::m_parse_server_name(Server& new_server, std::list<std::string>& line)
{
	if (line.size() != 1)
		throw std::invalid_argument("invalid config file");
	line.pop_front();
	new_server.server_name = line.front();
	line = m_next_line(0);
}

void		Config::m_parse_root(std::list<std::string>& line, Location& loc)
{
	if (line.size() != 2)
		throw std::invalid_argument("invalid config file");
	loc.root = line.back();
}

void		Config::m_parse_index(std::list<std::string>& line, Location& loc)
{
			line.pop_front();
			if (line.empty())
				throw std::invalid_argument("invalid config file");
			loc.v_index.clear();
			while (line.size())
			{
				loc.v_index.push_back(line.front());
				line.pop_front();
			}
}

void		Config::m_parse_allow_methods(std::list<std::string>& line, Location& loc)
{
			const char *method[3] = {"GET", "POST", "DELETE"};
			line.pop_front();
			if (line.empty())
				throw std::invalid_argument("invalid config file");
			loc.methods = 0;
			while (line.size())
			{
				int bit = 0x01;
				for (int i = 0; i < 3; i++)
				{
					if (line.front() == method[i])
						loc.methods |= bit;
					bit <<= 1;
				}
				line.pop_front();
			}
}

void		Config::m_parse_error_page(std::list<std::string>& line, Location& loc)
{
			line.pop_front();
			if (line.size() < 2)
				throw std::invalid_argument("invalid config file");
			loc.p_error_page.second = line.back();
			line.pop_back();
			loc.p_error_page.first.clear();
			while (line.size())
			{
				int code = atoi(line.front().c_str());
				m_is_valid_error_code(code);
				loc.p_error_page.first.push_back(code);
				line.pop_front();
			}
}

void		Config::m_parse_clent_max_body_size(std::list<std::string>& line, Location& loc)
{
			if (line.size() < 2)
				throw std::invalid_argument("invalid config file");
			int size = atoi(line.back().c_str());
			if (size < 256 || size > 4096)
				throw std::invalid_argument("invalid config file");
			loc.client_max_body_size = size;
}

void		Config::m_parse_auto_index(std::list<std::string>& line, Location& loc)
{
			if (line.size() < 2 || (line.back() != "on" && line.back() != "off"))
				throw std::invalid_argument("invalid config file");
			loc.auto_index = line.back() == "on";
}

void		Config::m_parse_return(std::list<std::string>& line, Location& loc)
{
			line.pop_front();
			if (line.size() != 2 || atoi(line.front().c_str()) != 301)
				throw std::invalid_argument("invalid config file");
			loc.p_return.first = 301;
			loc.p_return.second = line.back();
}

void		Config::m_parse_cgi_extension(std::list<std::string>& line, Location& loc)
{
			if (line.size() != 2)
				throw std::invalid_argument("invalid config file");
			loc.cgi = line.back();
}

Location	Config::m_parse_location(std::list<std::string>& line, Location& loc, Server& new_server)
{
	while (line.front() != "}" && line.front() != "location")
	{
		if (line.front() == "listen" && __s_brace.size() == 1)
			m_parse_listen(new_server, line);
		else if (line.front() == "server_name" && __s_brace.size() == 1)
			m_parse_server_name(new_server, line);
		else if (line.front() == "root")
			m_parse_root(line, loc);
		else if (line.front() == "index")
			m_parse_index(line, loc);
		else if (line.front() == "allow_methods")
			m_parse_allow_methods(line, loc);
		else if (line.front() == "error_page")
			m_parse_error_page(line, loc);
		else if (line.front() == "client_max_body_size")
			m_parse_clent_max_body_size(line, loc);
		else if (line.front() == "auto_index")
			m_parse_auto_index(line, loc);
		else if (line.front() == "cgi_extension" && __s_brace.size() == 2)
			m_parse_cgi_extension(line, loc);
		else if (line.front() == "return" && __s_brace.size() == 2)
			m_parse_return(line, loc);
		else
			throw std::invalid_argument(line.front());
		line = m_next_line(0);
	}
	return loc;
}

void		Config::m_parse_server(std::list<std::string> &line)
{
	Server	new_server;
	line = m_next_line(1);
	Location	default_location;
	new_server.default_location = m_parse_location(line, default_location, new_server);
	while (line.front() != "}" && __l_file.size())
	{
		if (line.front() != "location" || line.size() != 2)
			throw std::invalid_argument("invalid config file");
		Location	new_location(new_server.default_location);
		__s_brace.push("location");
		new_location.route = line.back();
		line = m_next_line(1);
		new_server.location.push_back(m_parse_location(line, new_location, new_server));
		if (line.size() != 1)
			throw std::invalid_argument("invalid config file");
		line = m_next_line(0);
		__s_brace.pop();
	}
	__v_server_list.push_back(new_server);
	if (!__l_file.size())
		throw std::invalid_argument("invalid config file2");
	if (line.size() != 1)
		throw std::invalid_argument("invalid config file");
	line = m_next_line(0); // server } 닫는 부분
	__s_brace.pop();
}

void		Config::parse_file()
{
	std::list<std::string> line = __l_file.front();
	while (__l_file.size() > 1)
	{
		if (line.size() != 1 || line.front() != "server")
			throw std::invalid_argument("invalid config file");
		if (__s_brace.size())
			throw std::invalid_argument("invalid config file");
		__s_brace.push("server");
		m_parse_server(line);
	}
}

static std::list<std::string> split_line(std::string str)
{
	std::list<std::string>	list;
	std::istringstream		iss(str);
	std::string				buf;
	
	while(iss >> buf)
		list.push_back(buf);
	return (list);
}

void		Config::open_file(std::string& conf_name)
{
	std::ifstream	fs;

	fs.open(conf_name, std::ios::in);
	if (!fs.is_open())
	{
		std::string conf_root = getcwd(nullptr, 1000);
		conf_root += "/config/" + conf_name;
		fs.open(conf_root);
		if (!fs.is_open())
			throw std::invalid_argument("Conf file does not exist");
	}
	while (!fs.eof())
	{
		std::string str;
		getline(fs >> std::ws, str);
		str = str.substr(0, str.find('#'));
		std::list<std::string> list;
		if (str.size())
		{
			list = split_line(str);
			__l_file.push_back(list);
		}
	}
	fs.close();
}
