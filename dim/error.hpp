#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>

struct	arg_error:std::exception
{
	const char* what() const throw()
	{
		return ("Error : argument");
	}
};

struct	file_name_error:std::exception
{
	const char* what() const throw()
	{
		return ("Error : file name");
	}
};

struct	file_open_error:std::exception
{
	const char* what() const throw()
	{
		return ("Error : file open");
	}
};

struct	config_form_error:std::exception
{
	const char* what() const throw()
	{
		return ("Error : config file format");
	}
};

// struct	_error:std::exception
// {
// 	const char* what() const throw()
// 	{
// 		return ("Error : ");
// 	}
// };

// struct	_error:std::exception
// {
// 	const char* what() const throw()
// 	{
// 		return ("Error : ");
// 	}
// };

#endif
