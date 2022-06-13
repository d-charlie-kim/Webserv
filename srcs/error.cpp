#include "../includes/error.hpp"

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