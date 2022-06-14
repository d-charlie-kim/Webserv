#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

std::list<std::string> Config::m_next_line(int brace_check = 0)
{
	std::list<std::string> line;

	while (brace_check >= 0)
	{
		if (__file.size() > 1)
			__file.pop_front();
		line = __file.front(); 		
		if (brace_check && line.front() != "{")
			throw std::invalid_argument("invalid config file");
		brace_check--;
	}
	return (line);
}

std::list<std::string> Config::m_split_line(std::string str)
{
	std::list<std::string>	list;
	std::istringstream		iss(str);
	std::string buf;
	
	while(iss >> buf)
		list.push_back(buf);
	return (list);
}

Config::Config() { }
Config::Config(const Config &) { }
Config& Config::operator= (const Config &) { return *this; }

Config::~Config() { }


void		Config::open_file(std::string &conf_name)
{
	std::ifstream		fs;

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
		std::list<std::string> list = m_split_line(str); // 수정
		if (*(list.begin()->begin()) != '#' && str.size())
			__file.push_back(list);
	}
	fs.close();
}

void		Config::m_parse_listen(Server &new_server, std::list<std::string> &line)
{
	line.pop_front();
	if (line.empty())
		throw std::invalid_argument("invalid config file");
	std::string str = line.front();
	// : 으로 구분하지 않고 ' ' 으로 구분한 경우는 에러일까?
	size_t pos = str.rfind(':');
	if (pos != std::string::npos)
	{
		int port = atoi(str.substr(pos + 1).c_str());
		// 포트 허용범위 49152 이상의 번호는 동적할당, 클라이언트 연결에 사용
		if (port < 0 || port > 49151)
			throw std::invalid_argument("invalid config file");
		new_server.listen.second = port;
	}
	new_server.listen.first = str.substr(0, pos);
	line = m_next_line(0);
}

void		Config::m_parse_server_name(Server &new_server, std::list<std::string> &line)
{
	line.pop_front();
	if (line.size() != 1)
		throw std::invalid_argument("invalid config file");
	new_server.server_name = line.front();
	line = m_next_line(0);
}

void		Config::m_parse_root(std::list<std::string> &line, Location &loc)
{
	if (line.size() != 2)
		throw std::invalid_argument("invalid config file");
	loc.root = line.back();
}

void		Config::m_parse_index(std::list<std::string>& line, Location& loc){}
void		Config::m_parse_allow_methods(std::list<std::string>& line, Location& loc){}
void		Config::m_parse_error_page(std::list<std::string>& line, Location& loc){}
void		Config::m_parse_clent_max_body_size(std::list<std::string>& line, Location& loc){}
void		Config::m_parse_auto_index(std::list<std::string>& line, Location& loc){}
void		Config::m_parse_cgi_extension(std::list<std::string>& line, Location& loc){}
void		Config::m_parse_return(std::list<std::string>& line, Location& loc){}


void		Config::m_parse_server(std::list<std::string> &line)
{
	if (__brace.size())
		throw std::invalid_argument("invalid config file");
	__brace.push("server");
	Server	new_server;
	line = m_next_line(1);
	
	if (line.front() == "listen")
		m_parse_listen(new_server, line);
	if (line.front() == "server_name")
		m_parse_server_name(new_server, line);

	
	Location	default_location;
	new_server.default_location = m_parse_location(line, default_location);
	while (line.front() != "}" && __file.size())
	{
		if (line.front() != "location" || line.size() != 2)
			throw std::invalid_argument("invalid config file");
		Location	new_location = new_server.default_location;
		new_location.route = line.back();
		line = m_next_line(1);
		new_server.location.push_back(m_parse_location(line, new_location));
		line = m_next_line(0);
	}
	__server_list.push_back(new_server);
	if (line.front() != "}")
		throw std::invalid_argument("invalid config file");
	std::cout << __file.size() << std::endl;
	if (__file.size())
		line = m_next_line(0);
	__brace.pop();
	if (__brace.size())
		throw std::invalid_argument("invalid config file");
}

Location	Config::m_parse_location(std::list<std::string> &line, Location loc)
{
	while (line.front() != "}" && line.front() != "location")
	{
		if (line.front() == "root")
			m_parse_root(line, loc);
		else if (line.front() == "index")
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
		else if (line.front() == "allow_methods")
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
		else if (line.front() == "error_page")
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
		else if (line.front() == "client_max_body_size")
		{
			if (line.size() < 2)
				throw std::invalid_argument("invalid config file");
			int size = atoi(line.back().c_str());
			if (size < 256 || size > 4096)
				throw std::invalid_argument("invalid config file");
			loc.client_max_body_size = size;
		}
		else if (line.front() == "auto_index")
		{
			if (line.size() < 2 || (line.back() != "on" && line.back() != "off"))
				throw std::invalid_argument("invalid config file");
			loc.auto_index = line.back() == "on";
		}
		else if (line.front() == "cgi_extension")
		{
			if (line.size() != 2)
				throw std::invalid_argument("invalid config file");
			loc.cgi = line.back();
		}
		else if (line.front() == "return")
		{
			line.pop_front();
			if (line.size() != 2 || atoi(line.front().c_str()) != 301)
				throw std::invalid_argument("invalid config file");
			loc.p_return.first = 301;
			loc.p_return.second = line.back();
		}
		line = m_next_line(0);
	}
	return loc;
}

void		Config::m_is_valid_error_code(int code)
{
	if (code < 300)
		throw std::invalid_argument("invalid config file");
}

void		Config::parse_file()
{
	std::list<std::string> line = __file.front();
	while (__file.size() > 1)
	{
		if (line.size() != 1 || line.front() != "server")
			throw std::invalid_argument("invalid config file");
		m_parse_server(line);
	}
}




std::vector<Server> Config::get_server_list() const { return __server_list; }
