#ifndef CONNECT_HPP
#define CONNECT_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "File.hpp"
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <sstream>

struct Connect
{
    std::map<int, Client> clients;
    std::map<int, File> files;
    struct kevent* curr_event;
    std::map<int, Server> servers;
    std::vector<struct kevent> change_list;
};

#endif