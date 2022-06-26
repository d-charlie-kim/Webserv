#include <fstream>
#include <sstream>
#include "../includes/Request_parser.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include <iostream>

void	post_function(Request &req, Client &client);
void	delete_function(Request &req, Client &client);
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
	{
		request.status_code = 405;
		return ;
	}
	
	// 존재하지 않는 경로라면 404
	struct stat		status;
	if (stat(request.path.c_str(), &status))
	{
		request.status_code = 404;
		return ;
	}

	// 디렉토리면 405
	if (request.is_directory)
	{
		request.status_code = 405;
		return ;
	}

	// POST 요청에는 반드시 body 및 content-length가 필요함, 없을 시 411 length required
	if (!request.content_length)
		request.status_code = 411;

	if (request.content_length > request.location->client_max_body_size)
		request.status_code = 413;

	// TODO open filepath  O_CTREAT | O_EXCL

}