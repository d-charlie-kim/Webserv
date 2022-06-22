#include <fstream>
#include <sstream>
#include "../includes/Request_parser.hpp"
#include "../includes/Client.hpp"
#include <iostream>

void get_html(Client &client)
{
	Request_parser		request(client.request_msg);

	request.parse_request(client.server);

	// response 함수이름(request, client); 예정


	/* todo
	 [ ] status code 뭐가 필요할지 정리해서 전달할 것
	 [x] request client로 바꾸는 작업
	 [ ] 411 length required POST 일 때
	 [ ] 메소드별로 분기 
	 [ ] post 
	 [ ] delete
	*/
	return ;
}
