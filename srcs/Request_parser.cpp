#include "../includes/Request_parser.hpp"


Request_parser::Request_parser(std::string request_msg)
	: __request_msg(request_msg) { }
Request_parser::~Request_parser() { }

static int		parse_method(std::string str, int allowed_methods)
{
	int	method = 0;

	const char *methods[3] = {"GET", "POST", "DELETE"};
	int bit = 0x01;
	for(int i = 0; i < 3; i++)
	{
		if (methods[i] == str)
		{
			method |= bit;

		}
		bit <<= 1;
	}
	if (!method)
	{
		std::cout << "ERROR invalid method" << std::endl;
		// TODO GET POST DELETE 그 어느 메소드와도 일치하지 않았을 때의 에러처리
	}
	// 마지막으로 허용된 메소드인지 확인

	return (method & allowed_methods);
}

static Location* search_location_in_list(std::string str, std::vector<Location> &list)
{
	std::vector<Location>::iterator it = list.begin();
	if (str.empty())
		str = "/";

	for (; it != list.end(); it++)
	{
		if (it->route == str) {
			return &(*it);
		}
	}
	return NULL;
}

Location* Request_parser::search_location(std::string str, std::vector<Location> &list)
{
	// 쿼리스트링 무시
	str = str.substr(0, str.find("?"));

	// 파일 확장자 확인 및 디렉토리 여부 저장
	size_t		pos = str.rfind(".");
	std::string	extension;
	if (pos == std::string::npos)
		__request.is_directory = true;
	else
		extension = str.substr(str.find("."));

	// 로케이션 완전일치 확인
	Location *location = search_location_in_list(str, list);
	// 최상위 루트까지 상위 디렉토리 하나씩 일치 확인
	while (!location && str.size() > 1)
	{
		str = str.substr(0, str.rfind("/"));
		location = search_location_in_list(str, list);
	}

	// cgi 확인
	if (location->cgi.size() && extension == location->cgi)
		__request.is_cgi = true;
	return location;
}

std::list<std::string>		split_word(std::string str)
{
	std::stringstream	iss(str);
	std::string			buf;
	std::list<std::string> list;

	while(iss >> buf)
		list.push_back(buf);
	return list;
}

void	Request_parser::split_request_msg()
{
	size_t pos = __request_msg.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		// TODO 헤더 바디 구분 개행이 아예 없는 경우 에러처리
		return;
	}
	__request.body = __request_msg.substr(pos + 4);

	std::string header = __request_msg.substr(0, pos);

	pos = header.find("\r\n");
	std::string line;
	while (pos != std::string::npos)
	{
		line = header.substr(0, pos);
		header = header.substr(pos + 2);

		__l_file.push_back(split_word(line));
		pos = header.find("\r\n");
	}
	__l_file.push_back(split_word(header));
}


std::list<std::string> Request_parser::m_next_line()
{
	if (__l_file.size() > 1)
		__l_file.pop_front();
	return (__l_file.front());
}

void	Request_parser::parse_request(Server *server)
{
	// body 저장
	split_request_msg();
	__l_line = __l_file.front();
	if (!__l_line.size())
	{
		// TODO 첫줄이 빈 줄이면 에러 response 최소 메소드는 들어와야
		std::cout << "!" << std::endl;
		return ;
	}
	// 처음 등장하는 단어(=method) 저장, 로케이션 확정 후 메소드 해석할 예정
	std::string		method_str = __l_line.front();
	__l_line.pop_front();

	// parse location part
	// url 입력값 없어도 괜찮도록 디폴트 로케이션 설정
	Location	*location = &(server->default_location);
	__request.url = "/";

	if (__l_line.size())
	{
		// set location & url
		__request.url = __l_line.front();
		location = search_location(__l_line.front(), server->location);
	}
	__l_line.pop_front();
	__request.location = location;

	// 본격적인 메소드 해석
	int method = parse_method(method_str, location->methods);
	if (!method)
	{
		std::cout << "ERROR" << std::endl;
		// TODO 허용되지 않은 메소드 405
		return ;
	}
	__request.method = method;

	// 프로토콜 들어오지 않아도 이상없음
	// 하지만 들어왔다면 HTTP/1.1과 정확히 일치해야
	if (__l_line.empty() || __l_line.front() == "HTTP/1.1")
		__request.is_valid_version = true;
	// TODO 바로 에러 response 전송 가능할 듯

	__l_line = m_next_line();
	while (__l_file.size() > 1)
	{
		__l_line = m_next_line();
		if (__l_line.front() == "Content-Length:" && __l_line.size() == 2)
			__request.content_length = std::atoi(__l_line.back().c_str());
		if (__l_line.front() == "Connection:" && __l_line.size() == 2)
			__request.connection = __l_line.back();
	}

}

Request		Request_parser::get_request() const { return __request; }
