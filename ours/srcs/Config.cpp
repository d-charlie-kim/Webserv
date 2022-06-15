#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

Config::Config() { }
Config::~Config() { }

std::vector<Server> Config::get_server_list() const { return __v_server_list; }

// next_line 인자로 들어오는 숫자가 건너 뛰는 라인 수 라고 직관적으로 알 수 있게 되면 좋겠다.
std::list<std::string> Config::m_next_line(int brace_check = 0)
{
	std::list<std::string> line;

	while (brace_check >= 0)
	{
		if (__l_file.size() > 1)
			__l_file.pop_front();
		line = __l_file.front();
//		// if (brace_check && line.front() != "{" && line.size() != 1)
		if (brace_check && line.front() != "{")
			throw std::invalid_argument("invalid config file123");
		brace_check--;
	}
	return (line);
}
// { } 모두 next_line할때 뒤에 뭐가 있는지 체크하지 않는다.
// { 체크는 next_line에서 if문에 하나만 추가해주면 됨
// } 체크는 server location 각 하나씩인데...

void		Config::m_is_valid_error_code(int code)
{
	if (code < 300)
		throw std::invalid_argument("invalid config file444");
}

// // Location	Config::m_parse_directive(std::list<std::string>& line, Location& loc, Server& server)
Location	Config::m_parse_location(std::list<std::string>& line, Location& loc, Server& server)
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
		// 기존에는 잘못된 directive가 들어왔을 때 걸러주지 못한다.
		// 그래서 else로 그 때 throw 해준다.
		// 이 부분이 추가되고, 각 if문에 server때인지 location때인지 비교하는(해야하는 경우만) 케이스만 넣어주면
		// + 구분을 위해서는 stack에 location을 넣는 부분이 있어야 한다.
		// cgi, return은 location에만 있어야 하고 server_name과 location은 server에만 있어야 하는 경우까지 고려하여 파싱된다.
		line = m_next_line(0);
	}
	return loc;
}

// Location	Config::m_parse_location_tempppp(std::list<std::string>& line, Location& loc, Server& server)
// {
// 	if (line.front() != "location" || line.size() != 2)
// 			throw std::invalid_argument("invalid config file1");
// 	Location	new_location = server.default_location;
// 	__s_brace.push("location");
// 	new_location.route = line.back();
// 	line = m_next_line(1);
// 	server.location.push_back(m_parse_directive(line, new_location, server));
// 	line = m_next_line(0); // location } 닫는 부분
// 	__s_brace.pop();
// }

void		Config::m_parse_server(std::list<std::string> &line)
{
	// parse_file 로 이동
	// if (__s_brace.size())
	// 	throw std::invalid_argument("invalid config file");
	// __s_brace.push("server");
	Server	new_server;
	line = m_next_line(1);
	/*
		server 인지 아닌지 stack으로 체크가 되니까
		같이 while문 돌면서 체크해도 괜찮겠다.	
	*/
	// if (line.front() == "listen")
	// 	m_parse_listen(new_server, line);
	// if (line.front() == "server_name")
	// 	m_parse_server_name(new_server, line);
	Location	default_location;
	new_server.default_location = m_parse_location(line, default_location, new_server);
	// location 만들어서 넣어주는 이유

	while (line.front() != "}" && __l_file.size())
	{
//		// m_parse_location_tempppp(line, default_location, new_server);
		if (line.front() != "location" || line.size() != 2)
			throw std::invalid_argument("invalid config file1");
		Location	new_location = new_server.default_location;
		// 복사 생성자 쓰지 않고 대입 연산한 이유 + 그래도 되는가
		__s_brace.push("location");
		new_location.route = line.back();
		line = m_next_line(1);
		new_server.location.push_back(m_parse_location(line, new_location, new_server));

//		//if (line.size() != 1)
//		//	throw std::invalid_argument("invalid config file");
		line = m_next_line(0); // location } 닫는 부분
		__s_brace.pop();
	}
	__v_server_list.push_back(new_server);

	// 파일이 끝났거나, } 거나를 두번 중복으로 체크
	// --> 파일이 끝났을때 error, 아니면 반드시 } 인 경우 로 if 한번만 사용
	if (!__l_file.size())
		throw std::invalid_argument("invalid config file2");
	// std::cout << __l_file.size() << std::endl;
	// if (__l_file.size())

//	//if (line.size() != 1)
//	//	throw std::invalid_argument("invalid config file");
	line = m_next_line(0); // server } 닫는 부분
	__s_brace.pop();
	// 불필요하다고 생각. __s_brace.push()는 서버 파싱 시작할때 한번밖에 하지 않는다.
	// __s_brace.pop()도 여기서 한번밖에 해주지 않는다.
	// 서버 파싱 시작할때 brace size 체크를 하거나, 끝날때 하거나 둘 중 하나만 남긴다.
	// if (__s_brace.size())
	// 	throw std::invalid_argument("invalid config file");
}

void		Config::parse_file()
{
	std::list<std::string> line = __l_file.front();
	while (__l_file.size() > 1)
	{
		if (line.size() != 1 || line.front() != "server")
			throw std::invalid_argument("invalid config file3");
		if (__s_brace.size())
			throw std::invalid_argument("invalid config file4");
		__s_brace.push("server");
		m_parse_server(line);
	}
}

static std::list<std::string> split_line(std::string str)
{
	std::list<std::string>	list;
	std::istringstream		iss(str);
	std::string buf;
	
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
		// 위 위 줄 덕분에 굳이 # 체크 안해도 괜찮습니다만
		// 이 부분에서 abort에러가 뜨는데 어떻게 돌아갔을까잉
		// if (*(list.begin()->begin()) != '#' && str.size())
	}
	fs.close();
}
