#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <exception>

class ArgumentCountError : public std::exception
{
	public:
		const char* what() const throw();
};

class ArgumentPathError : public std::exception
{
	public:
		const char* what() const throw();
};

class OpenError : public std::exception
{
	public:
		const char* what() const throw();
};

class ConfigureFileWrongError : public std::exception
{
	private:
		std::string	__msg;
	public:
		ConfigureFileWrongError(std::string msg);
		const char* what() const throw();
};

#endif
