#include <map>
#include <vector>
#include <fstream>
#include "../includes/Response.hpp"

static std::string set_url(Request request, Response response)
{
	switch (request.status_code)
	{
		case 200:
			response.header = "200 OK HTTP/1.1"
			;
		case 404:
			response.header = "404 NotFound"
			response.url = "./www/404.html";
		case 300:
			response.header = "300 Redirection"
			response.url = "./www/300.html";
		...
		default:
			;
	}
}

method

POST -> 
DELETE -> statuscode = 400;
GET -> pass


void response(Client client, Request request)
{
	Response	response;



	if (request.status_code)
		set_body();
	else
	{
		if (request.is_cgi)
			// cgi part (무엇을 보내주는가)
		else if (request.is_directory)
		{
			std::string check_route = request.location->root + request.location->route + request.url.substr(0, request.url.find('?', 0));
			std::ifstream test_fs;
			test_fs.open(check_route);
			if (!test_fs.is_open())
				response.status_code = 404;
			test_fs.close();
			else
			{
				for (std::vector<std::string>::iterator iter = request.location->v_index.begin(); iter != request.location->v_index.end(); iter++)
				{
					std::string index_check = check_route + *iter;
					test_fs.open(index_check)
					if (test_fs.is_open())
					{
						response.status_code = 200;
						response.url = index_check;
						test_fs.close();
						break ;
					}
					test_fs.close();
				}
				if (response.status_code == 0 && request.location->autoindex)
				{
					// autoindex 파트 --> CGI 로?
					// 작성
				}
				else if (response.status_code == 0)
					response.status_code = 404;
			}
		}
		else
		{
			std::string check_route = request.url.substr(0, request.url.find('?', 0));
			std::ifstream test_fs;
			fs.open(check_route)
			if (!test_fs.is_open())
				response.status_code = 404;
			else
			{
				response.status_code = 200;
				response.url = check_route;
			}
			fs.close();
		}
	}
	set_url(Request request, Response response);
	//FILE 로 body 받아오기

	file_path  -->  string  --> body;
	client.respond_msg = response.header + "\r\n" + response.body;
}

/*
	Question

	redirection 은 프로세스가 어떻게 진행되는가
	status_code를 저장할 곳이 필요한데, 어디가 좋을까 
	directory일때 + CGI가 아닌 파일일때 똑같이 open 검증하는 과정이 들어가는데 이것을 합치는 방향(간결하게) 고민중
	내일까지  set_url 함수 완성 예정

	POST는 어디서 처리하는가
*/