

#include <fstream>
#include <sstream>
#include <iostream>
#include "../includes/Client.hpp"
/*
	request parsing 과정에서 확인된 status code
		- 400 Bad request
		- 405 Not allowed method
		- 505 HTTP Version Not Supporteds
	POST 과정에서 확인된 status code
		- 411 length required
		- 413 payload too large
*/

void	post_function(Request &request, Client &client)
{
	// 존재하지 않는 경로라면 404
	struct stat		status;
	if (stat(request.path.c_str(), &status))
	{
		request.status_code = 404;
		return ;
	}
	
	// 디렉토리거나 정적파일 요청이면 405
	if (request.is_directory || !request.is_cgi)
	{
		request.status_code = 405;
		return ;
	}

	// POST 요청에는 반드시 body 및 content-length가 필요함, 없을 시 411 length required
	if (!request.content_length)
    {
		request.status_code = 411;
        return ;
    }

    // 413 Payload Too Large
	if (request.content_length > request.location->client_max_body_size)
    {
		request.status_code = 413;
        return ;
    }

	if(request.is_cgi)
    {
		// TODO cgi 요청 방법 정리되면 추가할 예정
        // POST cgi는 두가지 경우 
        // 1. 어떤 파일을 업로드한 경우 => cgi 거친다음에 저장해줘야
        // 2. 양식을 바디로 전달하는 경우 => GET과 유사 cgi 거친다음 response body
        
        // Cgi cgi(Connect, Client);
        // cgi.
    }

}