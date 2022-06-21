#include "../includes/Config.hpp"
#include "../includes/Client.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <map>

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

void get_one(std::string& respond_msg)
{
    std::fstream fs("/Users/saoh/work_dir/Webserv/html_file/index.html");
    fs.seekg (0, fs.end);
    int length = fs.tellg();
    fs.seekg (0, fs.beg);
    char buf[length + 1];

    respond_msg = "HTTP/1.1 200 OK\n"
                  "Content-length: ";
    respond_msg += ft_itoa(length) + "\n\n";
    fs.read(buf, length);
    buf[length] = 0;
    respond_msg += buf;
}

static int set_server(Server& server, int& reuse)
{
	int server_socket;
    struct sockaddr_in server_addr;

    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("socket() error\n" + std::string(strerror(errno)));
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server.listen.first.c_str());
    server_addr.sin_port = htons(server.listen.second);
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
		throw std::runtime_error("setsockopt error\n" + std::string(strerror(errno)));
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1)
		throw std::runtime_error("setsockopt error\n" + std::string(strerror(errno)));
    if (bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
		throw std::runtime_error("bind() error\n" + std::string(strerror(errno)));
    if (listen(server_socket, 1024) == -1)
		throw std::runtime_error("listen() error\n" + std::string(strerror(errno)));
    fcntl(server_socket, F_SETFL, O_NONBLOCK);
	return (server_socket);
}

static std::map<int, Server> connect_server(std::vector<Server>& server_list)
{
	std::map<int, Server>	servers;
	int reuse = 1;

	for (std::vector<Server>::iterator start = server_list.begin(); start != server_list.end(); start++)
		servers.insert(std::make_pair(set_server(*start, reuse), *start));
	return (servers);
}

static void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

static void set_events_servers(std::vector<struct kevent>& change_list, std::map<int, Server> servers)
{
	for (std::map<int, Server>::iterator iter = servers.begin(); iter != servers.end(); iter++)
		change_events(change_list, (*iter).first, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

static void disconnect_client(int client_fd, std::map<int, Client>& clients)
{
    std::cout << "client disconnected: " << client_fd << std::endl;
    close(client_fd);
    clients.erase(client_fd);
}

static void event_error(int error_fd, std::map<int, Server> servers, std::map<int, Client>& clients)
{
    if (servers.find(error_fd) != servers.end())
    {
        Server* tmp_server = &servers[error_fd];
        int reuse = 1;
        std::cerr << "server socket error" << std::endl;
        close(error_fd);
        servers.erase(error_fd);
        std::cerr << "server socket reopen" << std::endl;
        servers.insert(std::make_pair(set_server(*tmp_server, reuse), *tmp_server));
    }
    else
    {
        std::cerr << "client socket error" << std::endl;
        disconnect_client(error_fd, clients);
    }
}

static void write_data_to_client(int client_fd, std::vector<struct kevent>& change_list, std::map<int, Client>& clients)
{
    std::map<int, Client>::iterator it = clients.find(client_fd);
    if (it != clients.end())
    {
        get_one(clients[client_fd].respond_msg);
        if (clients[client_fd].respond_msg != "")
        {
            int n;
            if ((n = write(client_fd, clients[client_fd].respond_msg.c_str(),
                    clients[client_fd].respond_msg.size()) <= 0))
            {
                if (n < 0)
                    std::cerr << "client write error!" << std::endl;
                disconnect_client(client_fd, clients);
            }
            else
            {
                std::cout << "client " << client_fd << " write data"<< std::endl;
                clients[client_fd].request_msg.clear();
                clients[client_fd].respond_msg.clear();
                if (clients[client_fd].keep)
                    change_events(change_list, client_fd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
                else
                    disconnect_client(client_fd, clients);
            }
        }
    }
}

static void read_data_from_client(struct kevent* curr_event, std::map<int, Client>& clients)
{
    if (curr_event->data == 0) // read event 가 계속 발생 (keep-alive 로 열어뒀을때)
        return ;
    char buf[curr_event->data + 1];


    int n = read(curr_event->ident, buf, curr_event->data);
    if (n <= 0)
    {
        if (n < 0)
            std::cerr << "client read error!" << std::endl;
        disconnect_client(curr_event->ident, clients);
    }
    else
    {
        buf[n] = 0;
        std::cout << "client " << curr_event->ident << " read data"<< std::endl;
        clients[curr_event->ident].request_msg += buf;
    }
}

static void parsing_request_msg(Client& client, std::vector<struct kevent>& change_list, int client_fd)
{
    if (client.request_msg == "")
        return ;
    size_t bound = client.request_msg.find("\r\n\r\n");
    std::string header = client.request_msg.substr(0, bound);
    std::string body = client.request_msg.substr(bound + 4);
    std::istringstream iss_header(header);
    std::string tmp;

    change_events(change_list, client_fd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
    while (1)
    {
        iss_header >> tmp;
        if (iss_header.fail())
            break ;
        if (tmp == "Content-Length:")
        {
            size_t size;
            iss_header >> size;
            if (body.size() < size)
                change_events(change_list, client_fd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
        }
        if (tmp == "Connection:")
        {
            iss_header >> tmp;
            if (tmp == "close") // keep alive 면 타임체크도 해야 할 듯 하다.
                client.keep = false;
        }
    }
}

static void get_client(int server_fd, std::vector<struct kevent>& change_list, std::map<int, Client>& clients, Server& server)
{
    int client_socket;
    if ((client_socket = accept(server_fd, NULL, NULL)) == -1)
    {
        std::cerr << "client accept error!" << std::endl;
		return ;
    }
    std::cout << "get client : " << client_socket << std::endl;
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    change_events(change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    change_events(change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
    clients.insert(std::make_pair(client_socket, Client(&server)));
}

static void start_server(int& kq, std::map<int, Server>& servers, std::vector<struct kevent>& change_list)
{
    std::map<int, Client> clients;
    struct kevent event_list[8];
    int new_events;
    struct kevent* curr_event;

    std::cout << "echo server started" << std::endl;
	while (1)
    {
        new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
        if (new_events == -1)
			throw std::runtime_error("kevent() error\n" + std::string(strerror(errno)));
        change_list.clear();
        for (int i = 0; i < new_events; ++i)
        {
            curr_event = &event_list[i];
            if (curr_event->flags & EV_ERROR)
                event_error(curr_event->ident, servers, clients);
            else if (curr_event->filter == EVFILT_READ)
            {
                if (servers.find(curr_event->ident) != servers.end())
                    get_client(curr_event->ident, change_list, clients, servers[curr_event->ident]);
                else if (clients.find(curr_event->ident)!= clients.end())
                {
                    read_data_from_client(curr_event, clients);
                    parsing_request_msg(clients[curr_event->ident], change_list, curr_event->ident);
                }
            }
            else if (curr_event->filter == EVFILT_WRITE)
                write_data_to_client(curr_event->ident, change_list, clients);
        }
    }
}

void exec_server(std::vector<Server>& server_list)
{
	std::map<int, Server> servers = connect_server(server_list);
	int kq;
    if ((kq = kqueue()) == -1)
		throw std::runtime_error("kqueue() error\n" + std::string(strerror(errno)));
    std::vector<struct kevent> change_list;
    set_events_servers(change_list, servers);
    start_server(kq, servers, change_list);
}