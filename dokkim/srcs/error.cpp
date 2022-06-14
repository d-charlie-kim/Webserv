#include "../includes/error.hpp"
#include <exception>
#include <string>

const char* ArgumentCountError::what() const throw()
{
	return ("ERROR: Too Many Arguments");
};

const char* ArgumentPathError::what() const throw()
{
	return ("ERROR: Invalid Configure File Path");
};

const char* OpenError::what() const throw()
{
	return ("ERROR: Open Failed");
};

ConfigureFileWrongError::ConfigureFileWrongError(std::string msg) :__msg(msg) {};
ConfigureFileWrongError::~ConfigureFileWrongError() _NOEXCEPT {};
const char* ConfigureFileWrongError::what() const throw()
{
	return (__msg.c_str());
};
