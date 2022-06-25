#include "../includes/Cgi.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GET

Cgi::Cgi() {}

Cgi::Cgi(Connect& connect, Client& cur_client, const Request& request)
: __request(request),
	__v_envlist(std::vector<std::string>(15, ""))
{
	__cwd = m_get_cwd();
	__filepath = m_get_filepath();
	__requested_uri = __cwd + __filepath;
	__query_string = m_get_query_string();
	__v_envlist[0] = "REQUEST_METHOD=" + m_get_method(__request.method);
	__v_envlist[1] = "CONTENT_LENGTH=" + m_itoa(__request.content_length);
	__v_envlist[2] = "CONTENT_TYPE=" +  ; //request헤더 content_type 추가하기
	__v_envlist[3] = "GATEWAY_INTERFACE=CGI/1.1";
	__v_envlist[4] = "DOCUMENT_ROOT=" + __cwd;
	__v_envlist[5] = "PATH_TRANSLATED=" + __requested_uri;
	__v_envlist[6] = "QUERY_STRING=" + __query_string;
	__v_envlist[7] = "SCRIPT_NAME=" + __filepath;
	__v_envlist[8] = "SERVER_NAME=" + client.server.listen.first;
	__v_envlist[9] = "SERVER_PORT=" + m_itoa(client.server.listen.second);
	__v_envlist[10] = "SERVER_PROTOCOL==HTTP/1.1";
	__v_envlist[11] = "SERVER_SOFTWARE=dimteamwebserv";
	__v_envlist[12] = "REQUEST_URI=" + __requested_uri;
	__v_envlist[13] = "PATH_INFO=" + __requested_uri ;
	__v_envlist[14] = "SCRIPT_FILENAME=" + __requested_uri ; 
}

Cgi::~Cgi() {}

std::string	Cgi::m_get_cwd()
{
	char cwd[1000];
	getcwd(cwd, 1000);
	return ((std::string)cwd);
}

std::string Cgi::m_get_filepath()
{
	if (m_get_method(__request.method) == "POST")
		return (__request.url);
	return (__request.url.substr(0, __request.url.find_last_of('?')));
}

std::string Cgi::m_get_query_string()
{
	if (m_get_method(__request.method) == "POST")
		return ("");
	return (__request.url.substr(__request.url.find_last_of('?') + 1));
}

std::string	Cgi::m_get_method(int method)
{
	switch method
	{
		case GET :
			return ("GET");
		case POST :
			return ("POST");
		default :
			return ("");
	}
}

std::string	Cgi::m_itoa(int n)
{
	std::string result;

	if (n == 0)
		return ("");
	while (n)
	{
		result += n % 10 + '0';
		n /= 10;
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

void	Cgi::m_set_env()
{
	__env = new char*[__v_envlist.size()];
	for (int i = 0; i < __v_envlist.size(); i++)
	{
		__env[i] = new char[__v_envlist[i].size()];
		strcpy(__env[i], __v_envlist[i].c_str());
	}
	__env[__v_envlist.size()] = nullptr;
}

void	Cgi::m_set_argv()
{
	std::string cgi_path = __cwd + "/cgi-bin/php-cgi"; //cgi exec 변경
	__argv[0] = new char[__cwd.size() + cgi_path.size()];
	strcpy(__argv[0], cgi_path	.c_str());
	__argv[1] = new char[__requested_uri.size()];
	strcpy(__argv[1], __requested_uri.c_str());
	__argv[2] = nullptr;
}

void	Cgi::m_delete()
{
	for (int i = 0; i < __v_envlist.size(); i++)
		delete[] __env[i];
	delete[] __env;
	delete[] __argv[0];
	delete[] __argv[1];
}

std::string Cgi::m_cgi_exec()
{
	int		pipe_in[2];
	int 	pipe_out[2];
	pid_t	pid;

	m_set_env();
	m_set_argv();
	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
		return ("500");
	if ((pid = fork()) == -1)
		return ("500");
	else if (pid == 0)
	{
		close(pipe_in[WRITE]);
		close(pipe_out[READ]);
		dup2(pipe_in[READ], STDIN_FILENO);
		dup2(pipe_out[WRITE], STDOUT_FILENO);
		close(pipe_in[READ]);
		close(pipe_out[WRITE]);
		execve(__argv[0], __argv, __env);
		exit(1);
	}
	close(pipe_in[READ]);
	close(pipe_out[WRITE]);
	// 아래 구문을 event처리시 확인(오류시 리턴값 때문), pid 저장 필요??
	// int status;
	// waitpid(pid, &status, 0);
	// if (WIFEXITED(status) && WEXITSTATUS(status)) //wifexited:0이 아니면 자식 정상종료, wexitstatus:자식 exit반환값
	// 	return ("500");
	change_events(changs_list, pipe_in[WRITE], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL); // write가 더이상 필요한가??
	change_events(change_list, pipe_out[READ], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	Client c1;
	Client c2;
	c1.origin_fd = connect.clients[connect.curr_event.ident]; //현재 클라이언트 socket fd
	c2.origin_fd = connect.clients[connect.curr_event.ident]; //현재 클라이언트 socket fd
	c1.stage = CGI_WRITE;
	c2.stage = CGI_READ;
	client.insert(std::make_pair(pipe_in[WRITE], c1));
	client.insert(std::make_pair(pipe_out[READ], c2));
	client._stage = WAIT;
	m_delete();
	return ("200");
}
