#include "../includes/Cgi.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GET

Cgi::Cgi() {}

Cgi::Cgi(Connect& connect, Client, const Request& request)
: __cur_client_fd(cur_client_fd), __request(request),
	__v_envlist(std::vector<std::string>(15, ""))
{
	__cwd = m_get_cwd(); // 현재 작업 경로
	__filepath = m_get_filepath(); // requested uri(before '?')
	__requested_uri = __cwd + __filepath; // full requested uri
	__query_string = m_get_query_string();
	__v_envlist[0] = "REQUEST_METHOD=" + m_get_method(__request.method);
	__v_envlist[1] = "CONTENT_LENGTH=" + m_itostr(); //request.content_len, 0일땐 없는경우니 ""넣어주기
	
	__v_envlist[2] = "CONTENT_TYPE=" + ; //request헤더 content_type
	__v_envlist[3] = "GATEWAY_INTERFACE=CGI/1.1"; //cgi 버전
	__v_envlist[4] = "DOCUMENT_ROOT=" + __cwd;
	__v_envlist[5] = "PATH_TRANSLATED=" + __requested_uri; //translated version of the path given by the variable PATH_INFO
	__v_envlist[6] = "QUERY_STRING=" + __query_string; //the data following "?"
	__v_envlist[7] = "SCRIPT_NAME=" + __filepath; //request.uri
	
	__v_envlist[8] = "SERVER_NAME=" + ; //server's hostname or IP addr //connect.cur_event.ident
	
	__v_envlist[9] = "SERVER_PORT=" + ; //host port
	__v_envlist[10] = "SERVER_PROTOCOL==HTTP/1.1"; //고려해야하는가?? 
	
	__v_envlist[11] = "SERVER_SOFTWARE=dimteamwebserv"; //서블릿이 실행중인 서블릿 컨테이너의 이름과 버전, 네이밍필요
	__v_envlist[12] = "REQUEST_URI=" + __requested_uri; // (=PATH_INFO)
	__v_envlist[13] = "PATH_INFO=" __requested_uri ; //(=PATH_TRANSLATED) 
	__v_envlist[14] = "SCRIPT_FILENAME=" + __requested_uri ; //=path+ request.uri 
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
	return (__request.url.substr(0, __request.url.find('?')));
}

std::string Cgi::m_get_query_string()
{
	return (__request.url.substr(__request.url.find('?') + 1));
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

std::string	Cgi::m_itostr()
{
	// std::stringstream	ss;
	// std::string			ret;

	// if (__request.content_length == 0)
	// 	return ("");
	// ss << __request.content_length;
	// ss >> ret;
	// return (ret);
}

void	Cgi::m_set_env()
{
	__env = new char*[__v_envlist.size()];
	for (int i = 0; i < __v_envlist.size(); i++)
	{
		__env[i] = new char[__v_envlist[i].size()];
		strcpy(__env[i], __v_envlist[i].c_str());
	}
	__env[__v_envlist.size()] = NULL;
}

void	Cgi::m_set_argv()
{
	std::string cgi_path = __cwd + "/cgi-bin/php-cgi"; //cgi exec 변경
	__argv[0] = new char[__cwd.size() + cgi_path.size()];
	strcpy(__argv[0], cgi_path	.c_str());
	__argv[1] = new char[__requested_uri.size()];
	strcpy(__argv[1], __requested_uri.c_str());
	__argv[2] = NULL;
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
	{ // 자식 프로세스
		close(pipe_in[WRITE]);
		close(pipe_out[READ]);
		dup2(pipe_in[READ], STDIN_FILENO);
		dup2(pipe_out[WRITE], STDOUT_FILENO);
		close(pipe_in[READ]);
		close(pipe_out[WRITE]);
		execve(__argv[0], __argv, __env); //	php-cgi, ////cgi-bin/test.php
		write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n\r\n", 37); // exit(1)과 다른 오류??
		exit(1);
	}
	close(pipe_in[READ]);
	close(pipe_out[WRITE]);
	// close(pipe_in[WRITE]); // ??
	// int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status)) //wifexited:0이 아니면 자식 정상종료, wexitstatus:자식 exit반환값
		return ("500");
	change_events(changs_list, pipe_in[WRITE], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL); // write가 더이상 필요한가??
	change_events(change_list, pipe_out[READ], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	Client c1;
	Client c2;
	c1.origin_fd = connect.clients[__cur_client_fd]; //현재 클라이언트 socket fd
	c2.origin_fd = connect.clients[__cur_client_fd]; //현재 클라이언트 socket fd
	c1.stage = CGI_WRITE;
	c2.stage = CGI_READ;
	client.insert(std::make_pair(pipe_in[WRITE], c1));
	client.insert(std::make_pair(pipe_out[READ], c2));
	mcl._stage = WAIT;
	close(pipe_out[READ]);
	m_delete();
	return ("200");
}
