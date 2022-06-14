#include "../includes/Config.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

// void classify(std::string readline)
// {
// 	/*
// 		bool server_flag;
// 		bool location_flag;

// 		server 나오고
// 		{ 나오면 s_flag = true;

// 		location 나오고
// 		{ 나오면 l_flag = true;

// 		} 나왔는데 l_flag 가 true 이면
// 		l_flag = false

// 		} 나왔는데 l_flag 가 false 이면
// 		s_flag = false;

// 		이렇게 한 server와 location에 대한 내용 구분


// 		server는 vector로 활용
// 		location 또한 vector로 활용.
// 	*/





// 	/*
// 		앞 공백이 다 잘려져 있는 상태

// 		\n(단순 개행), # (주석), { } (중괄호) 가 제일 앞에 있으면 해당 줄은 무시한다.

// 		enum으로 switch를 사용하든
// 		strcmp 사용 if elseif로 하나씩 비교하든
// 		현재 Server 구조체에 있는 변수가 10개.

// 		10개 각각 저장하는 함수를 만들어서 유효성 체크를 한다.
// 		[우리가 만든 conf파일이기에 유효성 체크를 하지 않아도 되면 저장하는 단순 함수로 사용한다]
// 		void save_port_number();
// 		void save_limit_body_size();
// 		...

// 		근데 index, methods 같은 경우에는 string이 여러개 들어올 수 있다.
// 		methods 같은 경우에 나중에 true false로 판단을 해줘야 하므로
// 		bool 로 3칸짜리 배열로 만드는 것도 고려
// 		index는 vector<string>로 만들거나 동적할당 해줘야 함.
// 	*/
// }

// void parsing(std::string file_path)
// {
// 	/*
// 		fopen
// 			error 체크
// 		open 잘 되면

// 		while (1) {
// 			getline(file descriptor >> std::ws, readline);
// 			// 앞 공백을 잘라서 받음.
// 			if (eof)
// 				break;
// 			if ()
// 			classify(readline); // 분류하는 곳으로 보내기
// 			다 읽을때까지 반복
// 		}
// 	*/
// }


void	print_location_list(Location &loc)
{
	std::cout << "\tlocation path : " << loc.route << std::endl;
	std::cout << "\tlocation root : " << loc.root << std::endl;
	std::cout << "\tindex : ";
	for(std::vector<std::string>::iterator it = loc.v_index.begin(); it != loc.v_index.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "\tallow_method : ";
	const char *method[3] = {"GET", "POST", "DELETE"};
	int	bit = 0x01;
	for (int i = 0; i < 3; i++)
	{
		if (loc.methods & bit)
			std::cout << method[i] << " ";
		bit <<= 1;
	}
	std::cout << std::endl;
	std::cout << "\terror_page : ";
	for(std::vector<int>::iterator it = loc.p_error_page.first.begin(); it != loc.p_error_page.first.end(); it++)
		std::cout << *it << " ";

	std::cout << loc.p_error_page.second << std::endl;
	std::cout << "\tclient_max : " << loc.client_max_body_size  << std::endl;
	std::cout << "\treturn : " << loc.p_return.first << " " << loc.p_return.second << std::endl;
	std::cout << "\tcgi : " << loc.cgi << std::endl;
	std::cout << "\tauto_index : " << loc.auto_index << std::endl;
}

void	print_server_list(std::vector<Server> &list)
{
	for (std::vector<Server>::iterator it = list.begin(); it != list.end(); it++)
	{
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "server_listen ip :" << it->listen.first << std::endl;
		std::cout << "server_listen port :" << it->listen.second << std::endl;
		std::cout << "server_name : " << it->server_name << std::endl;
		std::cout << "default location" << std::endl;
		print_location_list(it->default_location);
		for(std::vector<Location>::iterator loc=it->location.begin(); loc != it->location.end(); loc++)
		{
			std::cout << "-----------------------------------" << std::endl;
			std::cout << "location" << std::endl;
			print_location_list(*loc);
		}
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "-----------------------------------" << std::endl;

	}
}

int main(int argc, const char *argv[])
{
	Config parser;

	try
	{
		if (argc > 3)
			throw std::invalid_argument("too many arguments");

		std::string	path = (argc == 1) ? "./config/our_default.conf" : argv[1];
		size_t		pos = path.rfind('.');

		if (pos == std::string::npos || path.substr(pos) != ".conf")
			throw std::invalid_argument("invalid file extension");

		parser.open_file(path);
		// parser.print();
		parser.parse_file();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	std::vector<Server> server_list = parser.get_server_list();
	print_server_list(server_list);

	return (0);
}
