#include "../includes/Config.hpp"

static void set_status_code(std::map<int, std::pair<std::string, std::string> >& status_code)
{
	status_code.insert(std::make_pair(100, std::make_pair("Continue", "./www/100.html")));						// 
	status_code.insert(std::make_pair(101, std::make_pair("Switching Protocols", "./www/101.html")));			// 
	status_code.insert(std::make_pair(200, std::make_pair("OK", "./www/200.html")));							// 정상
	status_code.insert(std::make_pair(201, std::make_pair("Created", "./www/201.html")));						// 
	status_code.insert(std::make_pair(202, std::make_pair("Accepted", "./www/202.html")));						// 
	status_code.insert(std::make_pair(203, std::make_pair("Non-Authoritative Information", "./www/203.html")));	// 
	status_code.insert(std::make_pair(204, std::make_pair("No Content", "./www/204.html")));					// 
	status_code.insert(std::make_pair(205, std::make_pair("Reset Content", "./www/205.html")));					// 
	status_code.insert(std::make_pair(206, std::make_pair("Partial Content", "./www/206.html")));				// 
	status_code.insert(std::make_pair(300, std::make_pair("Multiple Choices", "./www/300.html")));				// 
	status_code.insert(std::make_pair(301, std::make_pair("Moved Permanently", "./www/301.html")));				// 
	status_code.insert(std::make_pair(302, std::make_pair("Found", "./www/302.html")));							// 
	status_code.insert(std::make_pair(303, std::make_pair("See Other", "./www/303.html")));						// 
	status_code.insert(std::make_pair(304, std::make_pair("Not Modified", "./www/304.html")));					// 
	status_code.insert(std::make_pair(305, std::make_pair("Use Proxy", "./www/305.html")));						// 
	status_code.insert(std::make_pair(307, std::make_pair("Temporary Redirect", "./www/307.html")));			// 
	status_code.insert(std::make_pair(400, std::make_pair("Bad Request", "./www/400.html")));					// 
	status_code.insert(std::make_pair(401, std::make_pair("Unauthorized", "./www/401.html")));					// 
	status_code.insert(std::make_pair(402, std::make_pair("Payment Required", "./www/402.html")));				// 
	status_code.insert(std::make_pair(403, std::make_pair("Forbidden", "./www/403.html")));						// 
	status_code.insert(std::make_pair(404, std::make_pair("Not Found", "./www/404.html")));						// 
	status_code.insert(std::make_pair(405, std::make_pair("Method Not Allowed", "./www/405.html")));			// Method 권한이 없을 때
	status_code.insert(std::make_pair(406, std::make_pair("Not Acceptable", "./www/406.html")));				// 
	status_code.insert(std::make_pair(407, std::make_pair("Proxy Authentication Required", "./www/407.html")));	// 
	status_code.insert(std::make_pair(408, std::make_pair("Request Timeout", "./www/408.html")));				// 
	status_code.insert(std::make_pair(409, std::make_pair("Conflict", "./www/409.html")));						// 
	status_code.insert(std::make_pair(410, std::make_pair("Gone", "./www/410.html")));							// 해당 html 파일이 존재하지 않을 때
	status_code.insert(std::make_pair(411, std::make_pair("Length Required", "./www/411.html")));				// 
	status_code.insert(std::make_pair(412, std::make_pair("Precondition Failed", "./www/412.html")));			// 
	status_code.insert(std::make_pair(413, std::make_pair("Payload Too Large", "./www/413.html")));				// 
	status_code.insert(std::make_pair(414, std::make_pair("URI Too Long", "./www/414.html")));					// 
	status_code.insert(std::make_pair(415, std::make_pair("Unsupported Media Type", "./www/415.html")));		// 
	status_code.insert(std::make_pair(416, std::make_pair("Range Not Satisfiable", "./www/416.html")));			// 
	status_code.insert(std::make_pair(417, std::make_pair("Expectation Failed", "./www/417.html")));			// 
	status_code.insert(std::make_pair(426, std::make_pair("Upgrade Required", "./www/426.html")));				// 
	status_code.insert(std::make_pair(500, std::make_pair("Internal Server Error", "./www/500.html")));			// 
	status_code.insert(std::make_pair(501, std::make_pair("Not Implemented", "./www/501.html")));				// 
	status_code.insert(std::make_pair(502, std::make_pair("Bad Gateway", "./www/502.html")));					// 
	status_code.insert(std::make_pair(503, std::make_pair("Service Unavailable", "./www/503.html")));			// 
	status_code.insert(std::make_pair(504, std::make_pair("Gateway Timeout", "./www/504.html")));				// 
	status_code.insert(std::make_pair(505, std::make_pair("HTTP Version Not Supported", "./www/505.html")));	// HTTP version이 1.1이 아닐 때
}

void set_html_file(std::map<int, std::pair<std::string, std::string> >& status_code)
{
//	std::map<int, std::pair<std::string, std::string> > status_code;
	set_status_code(status_code);

	std::ofstream wFile;
	for (std::map<int, std::pair<std::string, std::string> >::iterator iter = status_code.begin(); iter != status_code.end(); iter++)
	{
		wFile.open(iter->second.second);
		if (wFile.fail())
			std::cout << "fail to set html file\n"; //합칠때는 throw로 웹서브 실행조차 안되게.
		wFile << "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n	<title>";
		wFile << iter->first << " " << iter->second.first;
		wFile << "</title>\r\n</head>\r\n<body>\r\n	<h1>";
		wFile << iter->first << " " << iter->second.first;
		wFile << "</h1>\r\n</body>\r\n</html>\r\n";
		wFile.close();
	}
}
