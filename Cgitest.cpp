#include "./Cgitest.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

//for test
Cgi::Cgi()
: __v_envlist(std::vector<std::string>(23, ""))
{
	m_get_cwd();
	std::string route = "/cgi-bin/test.php";
	__requested_uri = __cwd + route; // requested uri(before ?)
	__v_envlist[0] = "CONTENT_LENGTH=50"; //request.content_len
	__v_envlist[1] = "CONTENT_TYPE=application/x-www-form-urlencoded"; //request.content_type
	__v_envlist[2] = "GATEWAY_INTERFACE=CGI/1.1"; //cgi 버전
	__v_envlist[3] = "DOCUMENT_ROOT=" + __cwd; //the directory from which static document are served
	__v_envlist[4] = "PATH_TRANSLATED=" + __requested_uri; //translated version of the path given by the variable PATH_INFO
	__v_envlist[5] = "QUERY_STRING=first=workingtest1&second=workingtest2" ; //query info from requested url(the data following "?")
	__v_envlist[6] = "REQUEST_METHOD=GET"; //request.method
	__v_envlist[7] = "SCRIPT_NAME=" + route; //request.uri
	__v_envlist[8] = "SERVER_NAME=127.0.0.1"; //server's hostname or IP addr
	__v_envlist[9] = "SERVER_PORT=4000"; //request.server.listen.second
	__v_envlist[10] = "SERVER_PROTOCOL=" + (std::string)"HTTP/1.1"; //check?? 
	__v_envlist[11] = "SERVER_SOFTWARE=WEBSERV1.1"; //서블릿이 실행중인 서블릿 컨테이너의 이름과 버전, 내맘대로
	__v_envlist[12] = "AUTH_TYPE=BASIC" ; // 서블릿을 보호하는데 사용되는 인증 스키마 이름 ex)BASIC, SSL or null => BASIC or 없어도 될듯?
	// __v_envlist[13] = "REMOTE_ADDR=" ; //requst.server.listen.first, cgi: remote addr
	__v_envlist[14] = "REQUEST_URI=" + route + "first=workingtest1&second=workingtest2"; //requst.cgi.uri + query_string
	__v_envlist[15] = "SCRIPT_FILENAME=" + __cwd + route; //=path+ request.uri 
	// __v_envlist[16] = "REDIRECT_STATUS=200" ; //200(jseo& wiki) ????
	// __v_envlist[17] = "PHP_SELF=" + route; //?????
	__v_envlist[18] = "HTTP_HOST=127.0.0.1:4000"; // ?? ex)127.0.0.1:8080
	__v_envlist[19] = "HTTP_USER_AGENT="; //클라이언트가 요청을 보내기 위해 사용중인 브라우저의 유형 및 버전
	__v_envlist[20] = "HTTP_ACCEPT_LANGUAGE="; //수신할 내용에 적합한 언어 정의 ex)ko-KR,ko;q=0.9,en;q=0.7
	__v_envlist[21] = "HTTP_ACCEPT_ENCODING="; //클라이언트로 리턴되는 내용에 대해 수행될수있는 인코딩 유형 ex)gzip, deflate, br
	__v_envlist[22] = "PATH_INFO=" + __requested_uri; //(=PATH_TRANSLATED) 
}

// Cgi::Cgi(const Cgi& other) {}

Cgi::~Cgi() {
	// delete __cwd1;
	for (int i = 0; i < __v_envlist.size(); i++)
		delete[] __env[i];
	delete[] __env;
	delete[] __argv[0];
	delete[] __argv[1];
}

// Cgi& Cgi::operator=(const Cgi& other) { }

void	Cgi::m_get_cwd()
{
	char cwd[1000];
	getcwd(cwd, 1000);
	__cwd = (std::string)cwd;
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
	strcpy(__argv[0], cgi_path.c_str());
	__argv[1] = new char[__requested_uri.size()];
	strcpy(__argv[1], __requested_uri.c_str());
	__argv[2] = NULL;
}

//for test => working
std::string Cgi::m_cgi_exec()
{
	int		pipe_in[2];
	int 	pipe_out[2];
	pid_t	pid;
	char	buf[2000]; //BUFFERSIZE 처리
	int		len = 0;

	m_set_env();
	m_set_argv();
	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
		return ("pipe error"); // error처리하기
	std::string ss("first=formfist&second=formsecond");
	write(pipe_in[WRITE], ss.c_str(), ss.size());
	if ((pid = fork()) == -1)
		return ("fork error"); //error처리하기
	else if (pid == 0)
	{ // in child
		close(pipe_in[WRITE]);
		close(pipe_out[READ]);
		dup2(pipe_in[READ], STDIN_FILENO);
		dup2(pipe_out[WRITE], STDOUT_FILENO);
		close(pipe_in[READ]);
		close(pipe_out[WRITE]);
		execve(__argv[0], NULL, __env);
		write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n\r\n", 37);
		exit(1);
	}
	close(pipe_in[READ]);
	close(pipe_out[WRITE]);
	write(pipe_in[WRITE], "dddd\n", 5);
	close(pipe_in[WRITE]);
	int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status))
		return ("502"); //error 바꾸기
	std::string	ret = "";
	while ((len = read(pipe_out[READ], buf, 50)) >= 0)
	{
		buf[len] = '\0';
		ret += std::string(buf, len);
		if (!len)
			break;
	}
	close(pipe_out[READ]);
	return (ret);
}

//#####change event#####
// std::string Cgi::m_cgi_exec()
// {
// 	int pipe_in[2];
// 	int pipe_out[2];
// 	pipe(pipe_in);
// 	pipe(pipe_out);
// 	std::cout << pipe_in[0] << std::endl;
// 	// fd : 4, parent 닫고 child 의 표준입력을 이것으로 설정
// 	std::cout << pipe_in[1] << std::endl;
// 		// fd : 5, child 닫기, kqueue를 통해 이벤트가 발생하면 parent 가 여기다 쓰면 됨
// 	std::cout << pipe_out[0] << std::endl;
// 		// fd : 6, child 닫기, kqueue를 통해 이벤트가 발생하면 parent 가 여기를 읽음
// 	std::cout << pipe_out[1] << std::endl;
// 		// fd : 7, parent 닫고 child 의 표준 출력을 이것으로 설정
// 	int t = fork();
// 	if (t == 0)
// 	{
// 		close(pipe_in[1]);
// 		close(pipe_out[0]);
// 		dup2(pipe_in[0], STDIN_FILENO);
// 		dup2(pipe_out[1], STDOUT_FILENO);
// 		// exec 로 보내기
// 	}
// 	close(pipe_in[0]);
// 	close(pipe_out[1]);
// 	change_events(change_list, pipe_in[1], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
// 	change_events(change_list, pipe_out[0], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
// 	files.insert(std::make_pair(pipe_in[1], f));
// 	files.insert(std::make_pair(pipe_out[0], f));
// }

int main()
{
	Cgi cgi;
	std::string ret;
	ret = cgi.m_cgi_exec();
	std::cout << "ret :\n" << ret << std::endl;
	// system("leaks a.out");
	return 0;
}

// 1. cgi 통신 사용할 파이프 만들기
// 2. 파이프 연결(nonblocking)
// 3. 환경변수 세팅
// 4. fork 하기
// 5. 자식함수에서 execve
// 6. 부모함수에서 read, write