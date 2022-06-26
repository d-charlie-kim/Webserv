#include "../includes/Request_parser.hpp"


Request_parser::Request_parser(std::string request_msg)
	: __request_msg(request_msg) { }
Request_parser::~Request_parser() { }

static int		parse_method(std::string str)
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
	return (method);
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
	// 로케이션 완전일치 확인
	Location *location = search_location_in_list(str, list);
	// 최상위 루트까지 상위 디렉토리 하나씩 일치 확인
	while (!location && str.size() > 1)
	{
		str = str.substr(0, str.rfind("/"));
		location = search_location_in_list(str, list);
	}

	// cgi 확인
	if (location->cgi.size() && str.substr(str.find('.')) == location->cgi)
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
		// 헤더 바디 구분 개행이 아예 없어서 400 Bad Request
		__request.status_code = 400;
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
	memset(&__request, 0, sizeof(Request));
	split_request_msg();
	if (__request.status_code)
		return ;
	__l_line = __l_file.front();
	if (!__l_line.size())
	{
		// 첫줄이 빈 줄이면 에러 response 최소 메소드는 들어와야 함 400 Bad Request
		__request.status_code = 400;
		return ;
	}
	// 처음 등장하는 단어(=method) 저장, 로케이션 확정 후 메소드 해석할 예정
	std::string		method_str = __l_line.front();
	__l_line.pop_front();

	// parse location part
	// url 입력값 없어도 괜찮도록 디폴트 로케이션 설정
	Location	*location = &(server->default_location);
	__request.url = "/";
	__request.path = "/";

	if (__l_line.size())
	{
		// set location & url & path
		__request.url = __l_line.front();
		__request.path = __request.url.substr(__request.url.find('?'));
		location = search_location(__request.path, server->location);
		__l_line.pop_front();	
	}
	__request.location = location;

	// 디렉토리 여부 확인 
	//NOTE ISDIR 매크로로 디렉토리 or 디렉토리가 아님을 저장할지
	//ISREG 매크로로 일반 파일 or 일반파일이 아님을 저장할지
	struct stat		status;
	stat(__request.path.c_str(), &status);
	if (S_ISDIR(status.st_mode))
		__request.is_directory = true;
	
	// 본격적인 메소드 해석
	int method = parse_method(method_str);
	if (!method)
	{
		__request.status_code = 400;
		return ;
	}
	{
	if (!method & location->methods)
	{
		// 405 not allowed method
		__request.status_code  = 405;
		return ;
	}
	__request.method = method;

	// 프로토콜 들어오지 않아도 이상없음
	// 하지만 들어왔다면 HTTP/1.1과 정확히 일치해야 아니면 505 HTTP Version Not Supported
	if (!__l_line.empty() && __l_line.front() == "HTTP/1.1")
		__request.status_code = 505;

	// post 메소드라면 반드시 content-length 헤더를 가져야 함
	bool post_must_have_content_length = !(__request.method & POST);
	// post 메소드가 아니면 항상 true

	
	__l_line = m_next_line();
	while (__l_file.size() > 1)
	{
		__l_line = m_next_line();
		if (__l_line.front() == "Content-Length:" && __l_line.size() == 2)
		{
			post_must_have_content_length = true;
			// GET 일때 Content-Length가 들어와도 nginx 200 확인함
			__request.content_length = std::atoi(__l_line.back().c_str());
			// TODO  if(__request.content_length < __request.body.size()) 더 읽도록 추가해야
		}
		if (__l_line.front() == "Connection:" && __l_line.size() == 2)
			__request.connection = __l_line.back();
		if (__l_line.front() == "Content-Type:" && __l_line.size() == 2)
			__request.content_type = __l_line.back();
	}

	if (!post_must_have_content_length)
		__request.status_code = 411;
}

Request		Request_parser::get_request() const { return __request; }
