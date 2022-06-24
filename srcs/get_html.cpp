#include <fstream>
#include <sstream>
#include "../includes/Request_parser.hpp"
#include "../includes/Client.hpp"
#include <iostream>

void	post_function(Request &req, Client &client);
/*
	request parsing 과정에서 확인된 status code
		- 400 Bad request
		- 405 Not allowed method
		- 505 HTTP Version Not Supporteds

	POST 과정에서 확인된 status code
		- 411 length required
		- 413 payload too large
*/

void get_html(Client &client)
{
	Request_parser		parser(client.request_msg);

	parser.parse_request(client.server);

	
	Request		request = parser.get_request();


	switch (request.method)
	{
		case GET:
			break;
		case POST:
			post_function(request, client);
			break;
		case DELETE:
			delete_function(request, client);
	}
	return ;
}

void	post_function(Request &request, Client &client)
{
	if(request.is_cgi)
		// TODO cgi 요청 방법 정리되면 추가할 예정
		;
	
	std::string file_path;
	if (request.is_directory)
		file_path = "??? "; 
	// TODO 없는 디렉토리라면 당연히 404
	// 있는 디렉토리라면 임의의 파일명 ex.new_file.html 로 만들 수 있지 않을까?
	// 다른 상태코드를 찾아볼것

	

	file_path = request.location.root + request.location->route; 
	// TODO 잘린 부분을 가져온거라서 url ? 기준으로 잘라서 쓸것


	// POST 요청에는 반드시 body 및 content-length가 필요함, 없을 시 411 length required
	if (!request.content_length)
		request.status_code = 411

	if (request.content_length > request.location->client_max_body_size)
		request.status_code = 413;

	// TODO open filepath  O_CTREAT | O_EXCL

}