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
	bool						auto_index;
	int							methods;
	int							client_max_body_size;
	std::string					root;
	std::string					route; //path
	std::vector<std::string>	v_index;
	std::pair<std::vector<int>, std::string>	p_error_page;
	std::pair<int, std::string>	p_return; //없을때 int값 0
	std::string					cgi;

	Location();
	~Location();
	Location(const Location& default);
};

struct Server
{
	std::pair<std::string, int>		__listen;
	std::string						__server_name;
	struct Location					__default_location;
	std::vector<struct Location>	__location;
};

class Configure {
	private:
		
	public:

};

#endif
