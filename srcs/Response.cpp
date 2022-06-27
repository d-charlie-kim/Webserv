#include "Client.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connect.hpp"

// TODO request 파트랑 이어지게 정리하기
// TODO kqueue 완벽 이해하기
// TODO CGI 과정 완벽 이해하기
// TODO 플래그로 auto_index 인지 판단하기.
/*
	respond 작성 stage
*/
// TODO return 리다이렉션 있을 때 처리해주는 부분 어디에 넣을지 생각하기, return 일때는 kevent를 거치지 않는다!

static std::string make_hyper_link(std::string path)
{
	DIR *dir_ptr = NULL;
	struct dirent *file = NULL;
	std::string a_tag = "";

	dir_ptr = opendir(path.c_str());
	if (!dir_ptr)
	{
		// error 몇번?
		// return ?
	}
	while(1)
	{
		file = readdir(dir_ptr);
		if (!file)
			break ;
		a_tag += "<a href=\"";
		//a_tag += 현재 디렉토리 주소 + file->d_name;
		a_tag += "\">";
		a_tag += file->d_name;
		a_tag += "<\a>\n";
	}
	return a_tag;
}

static void make_auto_index_page(Request& request, Response& response)
{
	std::string	auto_index; // FILE 거쳐서 읽어온 auto_index.html 파일
	std::string name_dir = request.url.substr(request.url.rfind('/')); // 해당 directory 이름
	std::string read_dir = make_hyper_link(request.location->root + request.url.substr(0, request.url.find('?', 0))); // readir 함수 사용해서 읽어온 파일 목록
	/*
		auto_index.html 을 읽어온다. (event 거쳐야함)
	*/
	response.body = "";
	std::size_t index = 0;
	index = auto_index.find("$1"); //$1 찾아서
	response.body += auto_index.substr(0, index) + name_dir; // $1을 디렉토리 이름으로 바꿔줌
	auto_index = auto_index.substr(index + 2);
	index = auto_index.find("$1");
	response.body += auto_index.substr(0, index) + name_dir; // 두번 나오니, 한번 더 반복
	auto_index = auto_index.substr(index + 2);
	index = auto_index.find("$2");
	response.body += auto_index.substr(0, index) + read_dir; // $2를 디렉토리 내의 파일 목록으로 바꿔줌
	response.body += auto_index.substr(index + 2) + "\r\n";

	response.header = "HTTP/1.1 200 OK";
}

static void method_get(Request& request, Response& response)
{
	std::ifstream test_fs;
	std::string check_route;
	if (request.is_cgi) // cgi 일때 --> cgi에서 FILE을 거친 후 다시 들어오는데, flag를 꺼주는 방식으로 들어가고 나오고 조절 가능
	{

	}
	else if (request.is_directory) // directory 일때 (파일이 아닐 때)
	{
		check_route = request.location->root + request.url.substr(0, request.url.find('?', 0)); // root + (url에서 ? 앞부분) 이 유효한지만 체크 (파일 열때랑 동일)
		test_fs.open(check_route);
		if (!test_fs.is_open())
			request.status_code = 404;
		else
		{
			test_fs.close();
			for (std::vector<std::string>::iterator iter = request.location->v_index.begin(); iter != request.location->v_index.end(); iter++) // location에 명시되어 있는 index페이지가 유효한지 하나씩 체크, 유효한거 처음 발견하면 file_path 저장 후 break;
			{
				std::string index_check = check_route + *iter;
				test_fs.open(index_check);
				if (test_fs.is_open())
				{
					request.status_code = 200;
					response.file_path = index_check;
					test_fs.close();
					break ;
				}
				test_fs.close();
			}
			if (request.status_code == 0 && request.location->auto_index) // index파일이 전부 유효하지 않은데(그래서 status_code가 0), auto_index는 켜져 있을 때
				make_auto_index_page(request, response);
			else if (request.status_code == 0) // index 파일이 전부 유효하지 않은데, auto_index도 꺼져 있을 때
				request.status_code = 404;
		}
	}
	else // 파일일때, root + (url에서 ? 앞부분) 이 유효한지만 체크, 유효하면 file_path에 넣어준다. 유효하지 않으면 404.
	{
		check_route = request.location->root + request.url.substr(0, request.url.find('?', 0));
		test_fs.open(check_route);
		if (!test_fs.is_open())
			request.status_code = 404;
		else
		{
			request.status_code = 200;
			response.file_path = check_route;
		}
		test_fs.close();
	}
}


static void method_post(Request&, Response&)
{

}

static void method_delete(Request&, Response&)
{

}

static void method_exe(Request& request, Response& response)
{
	if (request.method == GET)
		method_get(request, response);
	else if (request.method == POST)
		method_post(request, response);
	else if (request.method == DELETE)
		method_delete(request, response);
}

static std::string ft_itoa(int len)
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

static std::string set_response(Connect& cn, Request& request, Response& response)
{
	response.header = "HTTP/1.1 ";
	response.header += ft_itoa(request.status_code);
	response.header += cn.m_status_code[request.status_code].first; // status_code 는 connect 안의 client에 있을 예정
	response.file_path = cn.m_status_code[request.status_code].second;
	for (std::vector<int>::iterator iter = request.location->p_error_page.first.begin(); iter != request.location->p_error_page.first.end(); iter++)
	{
		if (*iter == request.status_code)
			response.file_path = request.location->p_error_page.second;
	}
	return ("");
}

//FIXME 인자들 정리
void response(Connect& cn, Client& client, Request& request)
{
	Response	response;

	if (!request.status_code)
		method_exe(request, response);
	// if (flag == true) // flag는 Client 안에 넣을 예정
	 	set_response(cn, request, response); // status_code에 따라서 읽어올 file_path를 넣어주는 부분, 앞선 단계에서 body가 이미 생겼을 경우(200 OK 상황 등) 헤더만 추가해주고 넘어간다.
	// connect에서 file_path로부터 읽어오기 flag = false 로 바꿔줌.
	client.respond_msg = response.header + "\r\n" + response.body;
	// flag = true; // 다시 true로 바꿔줌
}

