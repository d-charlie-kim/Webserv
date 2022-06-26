#include "../includes/Connect.hpp"

std::string ft_itoa(int len)
{
    std::string result;

    while (len)
    {
        result += len % 10 + '0';
        len /= 10;
    }
    std::reverse(result.begin(), result.end());
    return (result);
}