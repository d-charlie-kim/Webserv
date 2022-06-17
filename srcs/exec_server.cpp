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
#include <map>

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

static void event_error(int event_fd, std::map<int, Server> servers, std::map<int, Client>& clients)
{
    if (servers.find(event_fd) != servers.end())
        throw std::runtime_error("server_socket error\n");
    else
    {
        std::cerr << "client socket error" << std::endl;
        disconnect_client(event_fd, clients);
    }
}

static void write_data_to_client(int client_fd, std::map<int, Client>& clients)
{
    std::map<int, Client>::iterator it = clients.find(client_fd);
    if (it != clients.end())
    {
        clients[client_fd].respond_msg = get_html(clients[client_fd].request_msg);
        if (clients[client_fd].respond_msg != "")
        {
            int n;
            if ((n = write(client_fd, clients[client_fd].respond_msg.c_str(),
                    clients[client_fd].respond_msg.size()) == -1))
            {
                std::cerr << "client write error!" << std::endl;
                disconnect_client(client_fd, clients);  
            }
            else
            {
                clients[client_fd].respond_msg.clear();
                clients[client_fd].request_msg.clear();
            }
        }
    }
}

static void read_data_from_client(int client_fd, std::map<int, Client>& clients)
{
    char buf[1024];
    int n = read(client_fd, buf, sizeof(buf));
    if (n <= 0)
    {
        if (n < 0)
            std::cerr << "client read error!" << std::endl;
        disconnect_client(client_fd, clients);
    }
    else
    {
        buf[n] = '\0';
        clients[client_fd].request_msg += buf;
        std::cout << "received data from " << client_fd << ": " << clients[client_fd].request_msg << std::endl;
    }
}

static void get_client(int server_fd, std::vector<struct kevent>& change_list, std::map<int, Client>& clients, Server& server)
{
    int client_socket;
    if ((client_socket = accept(server_fd, NULL, NULL)) == -1)
		throw std::runtime_error("accept() error\n" + std::string(strerror(errno)));
    std::cout << "accept new client: " << client_socket << std::endl;
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    change_events(change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    change_events(change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
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
                    read_data_from_client(curr_event->ident, clients);
            }
            else if (curr_event->filter == EVFILT_WRITE)
                write_data_to_client(curr_event->ident, clients);
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