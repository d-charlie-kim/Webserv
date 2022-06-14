#include "Config.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
	if (argc < 1 || argc > 2)
		throw std::invalid_argument("The number of argument is incollect");
	if (argc == 1)
		Configure cf("default.conf");
	else
		Configure cf(argv[1]);
	
}
