#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include "../includes/Configure.hpp"
#include "../includes/Server.hpp"
#include "../includes/error.hpp"

void set_conf_server(std::string read_line, struct Configure& conf)
{

}

void set_conf_location(std::string read_line, struct Configure& conf)
{

}

static bool brace_check(std::string read_line, struct Configure& conf)
{
	std::string			keyword;
	std::stringstream	iss(read_line);
	iss >> keyword;

	if (keyword == "{")
	{
		if (conf.server_flag && !conf.location_flag && conf.s_brace.empty())
		{
			conf.s_brace.push(SERVER);
			if (!conf.v_server_list.front().v_location.empty())
				conf.v_server_list.push_back("default");
			return true;
		}
		else if (conf.server_flag && conf.location_flag && conf.s_brace.top() == SERVER)
		{
			conf.s_brace.push(LOCATION);
			conf.v_server_list.back().v_location.push_back(conf.v_server_list.back().default_location);
			return true;
		}
		else
			throw ConfigureFileWrongError();
	}
	else if (keyword == "}")
	{
		if (conf.server_flag && conf.location_flag && conf.s_brace.top() == LOCATION)
		{
			conf.s_brace.pop();
			conf.location_flag = false;
			return true;
		}
		else if (conf.server_flag && !conf.location_flag && conf.s_brace.top() == SERVER)
		{
			conf.s_brace.pop();
			conf.server_flag = false;
			return true;
		}
		else
			throw ConfigureFileWrongError();
	}
	return false;
}

static bool server_location_check(std::string read_line, struct Configure& conf)
{
	std::string			keyword;
	std::stringstream	iss(read_line);
	iss >> keyword;

	if (keyword == "server" && !conf.server_flag)
	{
		conf.server_flag = true;
		// 끝났는지 체크 안 끝났으면 에러
		return true;
	}
	else if (keyword == "location" && conf.server_flag && !conf.location_flag)
	{
		conf.location_flag = true;
		// 뒤에 더 없으면 error
		// 뒤 체크 해줘서 route에 넣어주기
		std::string route;
		iss >> route;
		conf.v_server_list.back().default_location.route = route;
		// route valid check + 뒤에 뭐 더 붙어 있으면 error
		return true;
	}
	return false;
}

static void classify(struct Configure& conf, std::string read_line)
{
	if (read_line[0] == '#')
		return ;

	std::string			keyword;
	std::stringstream	iss(read_line);
	iss >> keyword;

	if (server_location_check(read_line, conf))
		return ;
	else if (brace_check(read_line, conf))
		return ;
	else
	{
		if (conf.s_brace.top() == SERVER)
			set_conf_server(read_line, conf);
		else if (conf.s_brace.top() == LOCATION)
			set_conf_location(read_line, conf);
		else
			throw ConfigureFileWrongError();
	}
}

void parsing(struct Configure& conf)
{
	std::ifstream fs(conf.file_path.c_str());
	if (fs.fail())
		throw OpenError();

	std::string		read_line;
	while (true)
	{
		std::getline(fs >> std::ws, read_line);
		classify(conf, read_line);
		if (fs.eof())
			break ;
	}
	fs.close();
}
