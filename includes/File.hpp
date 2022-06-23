#ifndef FILE_HPP
#define FILE_HPP

#include "Client.hpp"
#include <string>
#include <fstream>
#include <map>

struct File
{
	Client* client;
    std::string file_name;

    File(Client *_client) : client(_client) {}
};

void read_file(int fd, std::map<int, File>& files);
void write_file(int fd, std::map<int, File>& files);

#endif
