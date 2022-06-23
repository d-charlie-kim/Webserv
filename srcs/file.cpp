#include "../includes/File.hpp"

void read_file(int fd, std::map<int, File>& files)
{
    std::fstream fs(files[fd].file_name.c_str());
    fs.seekg (0, fs.end);
    int length = fs.tellg();
    fs.seekg (0, fs.beg);
    char buf[length + 1];

    fs.read(buf, length);
    buf[length] = 0;
    files[fd].client->file_read += buf;
}

void write_file(int fd, std::map<int, File>& files)
{
    
}