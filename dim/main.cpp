#include "Configure.hpp"
#include "struct_server.hpp"
#include "error.hpp"


std::string	arg_check(int argc, char* argv[])
{
	if (argc == 1)
		return ("./default.conf");
	else if (argc == 2)
	{
		std::string filename(argv[1]);
		if (argv[1] == '\0' || filename.size() < 6
			|| filename.find(".conf", filename.size() - 5) == std::string::npos)
			throw(arg_error());
		return (filename);
	}
	else
		throw (arg_error());
}

int		main(int argc, char* argv[])
{
	try
	{
		Configure conf(arg_check(argc, argv));
		conf.m_parsing();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}

