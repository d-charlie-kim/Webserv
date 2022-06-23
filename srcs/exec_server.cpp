#include "../includes/Config.hpp"
#include "../includes/Client.hpp"
#include "../includes/File.hpp"
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

static void event_error(Connect& cn)
{
    if (cn.servers.find(cn.curr_event->ident) != cn.servers.end())
    {
        Server* tmp_server = &(cn.servers[cn.curr_event->ident]);
        int reuse = 1;
        std::cerr << "server socket error" << std::endl;
        close(cn.curr_event->ident);
        cn.servers.erase(cn.curr_event->ident);
        std::cerr << "server socket reopen" << std::endl;
        cn.servers.insert(std::make_pair(set_server(*tmp_server, reuse), *tmp_server));
    }
    else
    {
        std::cerr << "client socket error" << std::endl;
        disconnect_client(cn.curr_event->ident, cn.clients);
    }
}

static void write_data_to_client(Connect& cn)
{
    get_one(cn.clients[cn.curr_event->ident].respond_msg);
    if (cn.clients[cn.curr_event->ident].respond_msg != "")
    {
        int n;
        if ((n = write(cn.curr_event->ident, cn.clients[cn.curr_event->ident].respond_msg.c_str(),
                cn.clients[cn.curr_event->ident].respond_msg.size()) <= 0))
        {
            if (n < 0)
                std::cerr << "client write error!" << std::endl;
            disconnect_client(cn.curr_event->ident, cn.clients);
        }
        else
        {
            std::cout << "client " << cn.curr_event->ident << " write data"<< std::endl;
            cn.clients[cn.curr_event->ident].request_msg.clear();
            cn.clients[cn.curr_event->ident].respond_msg.clear();
            cn.clients[cn.curr_event->ident].tmp_buffer.clear();
            if (cn.clients[cn.curr_event->ident].keep)
            {
                // change_events(change_list, client_fd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
            }
            else
                disconnect_client(cn.curr_event->ident, cn.clients);
        }
    }
}

static void read_data_from_client(Connect& cn)
{
    if (cn.curr_event->data == 0) // read event 가 계속 발생 (keep-alive 로 열어뒀을때)
        return ;
    char buf[cn.curr_event->data + 1];


    int n = read(cn.curr_event->ident, buf, cn.curr_event->data);
    if (n <= 0)
    {
        if (n < 0)
            std::cerr << "client read error!" << std::endl;
        disconnect_client(cn.curr_event->ident, cn.clients);
    }
    else
    {
        buf[n] = 0;
        std::cout << "client " << cn.curr_event->ident << " read data"<< std::endl;
        cn.clients[cn.curr_event->ident].request_msg += buf;
    }
}

static void parsing_request_msg(Connect& cn)
{
    if (cn.clients[cn.curr_event->ident].request_msg == "")
        return ;
    size_t bound = cn.clients[cn.curr_event->ident].request_msg.find("\r\n\r\n");
    std::string header = cn.clients[cn.curr_event->ident].request_msg.substr(0, bound);
    std::string body = cn.clients[cn.curr_event->ident].request_msg.substr(bound + 4);
    std::istringstream iss_header(header);
    std::string tmp;

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
            {
                // change_events(cn.change_list, client_fd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
            }
        }
        if (tmp == "Connection:")
        {
            iss_header >> tmp;
            if (tmp == "close") // keep alive 면 타임체크도 해야 할 듯 하다.
                cn.clients[cn.curr_event->ident].keep = false;
        }
    }
}

static void file_and_pipe_read(Connect& cn)
{
    // read(cn.curr_event->ident, buf, buf_size);
    // cn.clients[cn.clients[cn.curr_event->ident].origin_fd].tmp_buffer += buf;
    // cn.clients[cn.clients[cn.curr_event->ident].origin.fd]._state = SET_RESOURCE;
    std::cerr << "file and pipe read" << std::endl;
    disconnect_client(cn.curr_event->ident, cn.clients);
}

static void file_and_pipe_write(Connect& cn)
{
    // std::string& tmp = cn.clients[cn.clients[cn.curr_event->ident].origin_fd].tmp_buffer;
    // write(cn.curr_event->ident, tmp.c_str(), tmp.size());
    // if (cn.clients[cn.curr_event->ident]._state != CGI_WRITE)
    //      cn.clients[cn.clients[cn.curr_event->ident].origin.fd]._state = SET_RESOURCE;
    std::cerr << "file and pipe write" << std::endl;
    disconnect_client(cn.curr_event->ident, cn.clients);
}

static void get_client(Connect& cn)
{
    int client_socket;
    if ((client_socket = accept(cn.curr_event->ident, NULL, NULL)) == -1)
    {
        std::cerr << "client accept error!" << std::endl;
		return ;
    }
    std::cout << "get client : " << client_socket << std::endl;
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    change_events(cn.change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    change_events(cn.change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
    cn.clients.insert(std::make_pair(client_socket, Client(&(cn.servers[cn.curr_event->ident]))));
}

static void start_server(int& kq, Connect& cn)
{
    struct kevent event_list[8];
    int new_events;

    std::cout << "echo server started" << std::endl;
	while (1)
    {
        new_events = kevent(kq, &(cn.change_list[0]), cn.change_list.size(), event_list, 8, NULL);
        if (new_events == -1)
			throw std::runtime_error("kevent() error\n" + std::string(strerror(errno)));
        cn.change_list.clear();
        for (int i = 0; i < new_events; ++i)
        {
            cn.curr_event = &event_list[i];
            if (cn.curr_event->flags & EV_ERROR)
                event_error(cn);
            else if (cn.curr_event->filter == EVFILT_READ)
            {
                if (cn.servers.find(cn.curr_event->ident) != cn.servers.end())
                    get_client(cn);
                else if (cn.clients.find(cn.curr_event->ident)!= cn.clients.end())
                {
                    if (cn.clients[cn.curr_event->ident]._stage == WAIT)
                        continue ;
                    else if (cn.clients[cn.curr_event->ident]._stage == GET_REQUEST)
                    {
                        read_data_from_client(cn);
                        parsing_request_msg(cn);
                    }
                    else if (cn.clients[cn.curr_event->ident]._stage == CGI_READ ||
                                cn.clients[cn.curr_event->ident]._stage && FILE_READ)
                        file_and_pipe_read(cn);
                }
            }
            else if (cn.curr_event->filter == EVFILT_WRITE)
            {
                if (cn.clients.find(cn.curr_event->ident) != cn.clients.end())
                {
                    if (cn.clients[cn.curr_event->ident]._stage == WAIT)
                        continue ;
                    else if (cn.clients[cn.curr_event->ident]._stage == SET_RESOURCE)
                    {
                    }
                    else if (cn.clients[cn.curr_event->ident]._stage == SEND_RESPONSE)
                        write_data_to_client(cn);
                    else if (cn.clients[cn.curr_event->ident]._stage == CGI_WRITE ||
                                cn.clients[cn.curr_event->ident]._stage && FILE_WRITE)
                        file_and_pipe_write(cn);
                }
            }
        }
    }
}

void exec_server(std::vector<Server>& server_list)
{
    Connect cn;
    cn.servers = connect_server(server_list);
	int kq;
    if ((kq = kqueue()) == -1)
		throw std::runtime_error("kqueue() error\n" + std::string(strerror(errno)));
    set_events_servers(cn.change_list, cn.servers);
    start_server(kq, cn);
}