#include "../includes/Config.hpp"
#include "../includes/Client.hpp"
#include <fstream>

void get_one(std::string& respond_msg)
{
    std::fstream fs("/Users/saoh/work_dir/Webserv/html_file/index.html");
    char buf[100000];

    respond_msg = "HTTP/1.1 200 OK"
                  "Content-length: 50\n\n";
    fs.read(buf, 100000);
    respond_msg += buf;
}

void get_two(std::string& respond_msg)
{
    std::fstream fs("/Users/saoh/work_dir/Webserv/html_file/404.html");
    char buf[100000];


    respond_msg = "HTTP/1.1 200 OK"
                  "Content-length: 50\n\n";
    fs.read(buf, 100000);
    respond_msg += buf;
}

void get_three(std::string& respond_msg)
{
    std::fstream fs("/Users/saoh/work_dir/Webserv/html_file/autoindex.html");
    char buf[100000];

    respond_msg = "HTTP/1.1 200 OK"
                  "Content-length: 50\n\n";
    fs.read(buf, 100000);
    respond_msg += buf;
}

void get_four(std::string& respond_msg)
{
    std::fstream fs("/Users/saoh/work_dir/Webserv/html_file/index_second.html");
    char buf[100000];

    respond_msg = "HTTP/1.1 200 OK"
                  "Content-length: 50\n\n";
    fs.read(buf, 100000);
    respond_msg += buf;
}

std::string get_html(std::string request_msg)
{
    std::string respond_msg("");

    if (request_msg.find("GET / HTTP/1.1") != std::string::npos)
        get_one(respond_msg);
    else if (request_msg.find("GET /aa/ HTTP/1.1") != std::string::npos)
        get_two(respond_msg);
    else if (request_msg.find("GET /aa/bb/ HTTP/1.1") != std::string::npos)
        get_three(respond_msg);
    else if (request_msg.find("GET /aa/bb/cc/ HTTP/1.1") != std::string::npos)
        get_four(respond_msg);
    else
        respond_msg = request_msg;
    return (respond_msg);
}