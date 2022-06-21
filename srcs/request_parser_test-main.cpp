#include "../includes/Config.hpp"
#include "../includes/Request_parser.hpp"

std::string mock_request_str()
{
	std::ifstream fs;
	fs.open("string.txt");

	std::string		request;
	std::string		buf;

	while(!fs.eof())
	{
		std::getline(fs, buf);
		request += (buf + "\n") ;
	}
	fs.close();
	return request;
}



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

int main()
{
	Config parser;

	try
	{
		std::string path = "config/our_default.conf";
		parser.open_file(path);
		parser.parse_file();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	Server	*server = &*parser.get_server_list().begin();
	Request_parser request_parser(mock_request_str());




	request_parser.parse_request(server);


	Request req = request_parser.get_request();
	{
		std::cout << "method " << req.method << " ";
		const char *method[3] = {"GET", "POST", "DELETE"};
		int	bit = 0x01;
		for (int i = 0; i < 3; i++)
		{
			if (req.method & bit)
				std::cout << method[i] << " ";
			bit <<= 1;
		}
		std::cout << std::endl;
		std::cout << "location";
		print_location_list(*req.location);
		std::cout << "url : " << req.url << std::endl;
		std::cout << "is_valid_version : " << req.is_valid_version << std::endl;
		std::cout << "is_cgi : " << req.is_cgi << std::endl;
		std::cout << "is_directory : " << req.is_directory << std::endl;
		std::cout << "connection : " << req.connection << std::endl;
		std::cout << "content_length : " << req.content_length << std::endl;
	}
}
