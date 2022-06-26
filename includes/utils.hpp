#ifndef UTILS_HPP
#define UTILS_HPP

#include "Client.hpp"
#include "Connect.hpp"


void request_msg_parsing(Client& client);
std::string ft_itoa(int len);
void response(Connect& cn, Client& client, Request& request);

#endif