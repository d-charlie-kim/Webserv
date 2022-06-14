#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include "../includes/Configure.hpp"
#include "../includes/Server.hpp"
#include "../includes/error.hpp"

static bool brace_check(std::string read_line, struct Configure& conf)
{
	std::string			keyword;
	std::stringstream	iss(read_line);
	iss >> keyword;

	if (keyword == "{")
	{
		iss >> keyword;
		if (iss.good())
			throw ConfigureFileWrongError("Strict rule about the \'Brace\'");
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
			throw ConfigureFileWrongError("ERROR, Configure File\n : \'Brace\' is something wrong");
	}
	else if (keyword == "}")
	{
		iss >> keyword;
		if (iss.good())
			throw ConfigureFileWrongError("ERROR, Configure File\n : Strict rule about the \'Brace\'");
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
			throw ConfigureFileWrongError("ERROR, Configure File\n : \'Brace\' is something wrong");
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
		iss >> keyword;
		if (iss.good())
			throw ConfigureFileWrongError("ERROR, Configure File\n : Something after \'server\' word");
		return true;
	}
	else if (keyword == "location" && conf.server_flag && !conf.location_flag)
	{
		conf.location_flag = true;

		std::string route;
		iss >> route;
		if (iss.fail())
			throw ConfigureFileWrongError("ERROR, Configure File\n : No route after \'location\'");
		conf.v_server_list.back().default_location.route = route;
		iss >> route;
		if (iss.good())
			throw ConfigureFileWrongError("ERROR, Configure File\n : Something after \'location \\route\'");
		//route path valid check
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
			set_conf_server(read_line, conf.v_server_list);
		else if (conf.s_brace.top() == LOCATION)
			set_conf_location(read_line, conf.v_server_list.back().v_location);
		else
			throw ConfigureFileWrongError("ERROR, Configure File\n : Wrong component");
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
