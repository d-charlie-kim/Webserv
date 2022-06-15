#include <iostream>
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

void		Config::m_parse_listen(Server &new_server, std::list<std::string> &line)
{
	line.pop_front();
	if (line.empty())
		throw std::invalid_argument("invalid config file13");
	std::string str = line.front();
	// : 으로 구분하지 않고 ' ' 으로 구분한 경우는 에러일까?
	size_t pos = str.rfind(':');
	if (pos != std::string::npos)
	{
		int port = atoi(str.substr(pos + 1).c_str());
		// 포트 허용범위 49152 이상의 번호는 동적할당, 클라이언트 연결에 사용
		if (port < 0 || port > 49151)
			throw std::invalid_argument("invalid config file12");
		new_server.listen.second = port;
	}
	new_server.listen.first = str.substr(0, pos);
	line = m_next_line(0);
}

void		Config::m_parse_server_name(Server &new_server, std::list<std::string> &line)
{
	line.pop_front();
	if (line.size() != 1)
		throw std::invalid_argument("invalid config file11");
	new_server.server_name = line.front();
	line = m_next_line(0);
}

void		Config::m_parse_root(std::list<std::string>& line, Location& loc)
{
	if (line.size() != 2)
		throw std::invalid_argument("invalid config file10");
	loc.root = line.back();
}

void		Config::m_parse_index(std::list<std::string>& line, Location& loc)
{
			line.pop_front();
			if (line.empty())
				throw std::invalid_argument("invalid config file9");
			loc.v_index.clear();
			while (line.size())
			{
				loc.v_index.push_back(line.front());
				line.pop_front();
			}
}

void		Config::m_parse_allow_methods(std::list<std::string>& line, Location& loc)
{
			const char *method[3] = {"GET", "POST", "DELETE8"};
			line.pop_front();
			if (line.empty())
				throw std::invalid_argument("invalid config file7");
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
				throw std::invalid_argument("invalid config file6");
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
				throw std::invalid_argument("invalid config file5");
			int size = atoi(line.back().c_str());
			if (size < 256 || size > 4096)
				throw std::invalid_argument("invalid config file4");
			loc.client_max_body_size = size;
}

void		Config::m_parse_auto_index(std::list<std::string>& line, Location& loc)
{
			if (line.size() < 2 || (line.back() != "on" && line.back() != "off"))
				throw std::invalid_argument("invalid config file3");
			loc.auto_index = line.back() == "on";
}

void		Config::m_parse_return(std::list<std::string>& line, Location& loc)
{
			line.pop_front();
			if (line.size() != 2 || atoi(line.front().c_str()) != 301)
				throw std::invalid_argument("invalid config file2");
			loc.p_return.first = 301;
			loc.p_return.second = line.back();
}

void		Config::m_parse_cgi_extension(std::list<std::string>& line, Location& loc)
{
			if (line.size() != 2)
				throw std::invalid_argument("invalid config file1");
			loc.cgi = line.back();
}

Location	Config::m_parse_directive(std::list<std::string>& line, Location& loc, Server& server)
{
	while (line.front() != "}" && line.front() != "location")
	{
		if (line.front() == "listen" && __s_brace.size() == 1)
			m_parse_listen(server, line);
		else if (line.front() == "server_name" && __s_brace.size() == 1)
			m_parse_server_name(server, line);
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
