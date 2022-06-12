#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>

// 변수만 있으면 구조체를 쓰면 되지 않느냐
// 생성자로 초기화 해줄 필요가 있을 수 있다.

/*
	conf파일에 명시되어 있지 않아서
	default 값이 필요한 경우가 있고,

	conf파일에 명시되어 있지 않으면
	error 처리를 해야하는 경우가 있다.


	--> 그래서 
		없으면 사용할 default 값을 넣어줘야 하고,
		있으면 거기에 덮어주면 된다.
		때문에 class를 사용하는게 더 나을 수 있다.
*/

/*
	하지만 Server가 여러개 나올 수 있기에,
	Vector 같은 컨테이너를 통해서 Server를 하나씩 저장해야 하는 것 같다.
*/

struct Location
{
	std::string root;
	std::string index; // 여러개 
	std::string	cgi;
	std::string methods; // 여러개
	// bool methods[3];
};

struct Server
{
	int			port_number;
	int			limit_body_size;
	bool		auto_index;
	std::string host;
	std::string server_name;
	std::string default_error_page;
	std::vector<struct Location> location;
};

class Config {
	private:
		int			port_number;
		int			limit_body_size;
		bool		auto_index;
		std::string	cgi;
		std::string host;
		std::string server_name;
		std::string default_error;
		std::string root;
		std::string methods;
	public:
	Config()
		: port_number(8080), limit_body_size(2000), ... {};
	~Config();
};

#endif
