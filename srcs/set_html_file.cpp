#include "Config.hpp"

static void set_first_line(std::map<int, std::pair<std::string, std::string> >& first_line)
{
	first_line.insert(std::make_pair(100, std::make_pair("Continue", "./www/100.html")));						// 
	first_line.insert(std::make_pair(101, std::make_pair("Switching Protocols", "./www/101.html")));			// 
	first_line.insert(std::make_pair(200, std::make_pair("OK", "./www/200.html")));							// 정상
	first_line.insert(std::make_pair(201, std::make_pair("Created", "./www/201.html")));						// 
	first_line.insert(std::make_pair(202, std::make_pair("Accepted", "./www/202.html")));						// 
	first_line.insert(std::make_pair(203, std::make_pair("Non-Authoritative Information", "./www/203.html")));	// 
	first_line.insert(std::make_pair(204, std::make_pair("No Content", "./www/204.html")));					// 
	first_line.insert(std::make_pair(205, std::make_pair("Reset Content", "./www/205.html")));					// 
	first_line.insert(std::make_pair(206, std::make_pair("Partial Content", "./www/206.html")));				// 
	first_line.insert(std::make_pair(300, std::make_pair("Multiple Choices", "./www/300.html")));				// 
	first_line.insert(std::make_pair(301, std::make_pair("Moved Permanently", "./www/301.html")));				// 
	first_line.insert(std::make_pair(302, std::make_pair("Found", "./www/302.html")));							// 
	first_line.insert(std::make_pair(303, std::make_pair("See Other", "./www/303.html")));						// 
	first_line.insert(std::make_pair(304, std::make_pair("Not Modified", "./www/304.html")));					// 
	first_line.insert(std::make_pair(305, std::make_pair("Use Proxy", "./www/305.html")));						// 
	first_line.insert(std::make_pair(307, std::make_pair("Temporary Redirect", "./www/307.html")));			// 
	first_line.insert(std::make_pair(400, std::make_pair("Bad Request", "./www/400.html")));					// 
	first_line.insert(std::make_pair(401, std::make_pair("Unauthorized", "./www/401.html")));					// 
	first_line.insert(std::make_pair(402, std::make_pair("Payment Required", "./www/402.html")));				// 
	first_line.insert(std::make_pair(403, std::make_pair("Forbidden", "./www/403.html")));						// 
	first_line.insert(std::make_pair(404, std::make_pair("Not Found", "./www/404.html")));						// 
	first_line.insert(std::make_pair(405, std::make_pair("Method Not Allowed", "./www/405.html")));			// Method 권한이 없을 때
	first_line.insert(std::make_pair(406, std::make_pair("Not Acceptable", "./www/406.html")));				// 
	first_line.insert(std::make_pair(407, std::make_pair("Proxy Authentication Required", "./www/407.html")));	// 
	first_line.insert(std::make_pair(408, std::make_pair("Request Timeout", "./www/408.html")));				// 
	first_line.insert(std::make_pair(409, std::make_pair("Conflict", "./www/409.html")));						// 
	first_line.insert(std::make_pair(410, std::make_pair("Gone", "./www/410.html")));							// 해당 html 파일이 존재하지 않을 때
	first_line.insert(std::make_pair(411, std::make_pair("Length Required", "./www/411.html")));				// 
	first_line.insert(std::make_pair(412, std::make_pair("Precondition Failed", "./www/412.html")));			// 
	first_line.insert(std::make_pair(413, std::make_pair("Payload Too Large", "./www/413.html")));				// 
	first_line.insert(std::make_pair(414, std::make_pair("URI Too Long", "./www/414.html")));					// 
	first_line.insert(std::make_pair(415, std::make_pair("Unsupported Media Type", "./www/415.html")));		// 
	first_line.insert(std::make_pair(416, std::make_pair("Range Not Satisfiable", "./www/416.html")));			// 
	first_line.insert(std::make_pair(417, std::make_pair("Expectation Failed", "./www/417.html")));			// 
	first_line.insert(std::make_pair(426, std::make_pair("Upgrade Required", "./www/426.html")));				// 
	first_line.insert(std::make_pair(500, std::make_pair("Internal Server Error", "./www/500.html")));			// 
	first_line.insert(std::make_pair(501, std::make_pair("Not Implemented", "./www/501.html")));				// 
	first_line.insert(std::make_pair(502, std::make_pair("Bad Gateway", "./www/502.html")));					// 
	first_line.insert(std::make_pair(503, std::make_pair("Service Unavailable", "./www/503.html")));			// 
	first_line.insert(std::make_pair(504, std::make_pair("Gateway Timeout", "./www/504.html")));				// 
	first_line.insert(std::make_pair(505, std::make_pair("HTTP Version Not Supported", "./www/505.html")));	// HTTP version이 1.1이 아닐 때
}

static void set_html_file(std::map<int, std::pair<std::string, std::string> >& first_line)
{
	set_first_line(first_line);

	std::ofstream wFile;
	for (std::map<int, std::pair<std::string, std::string> >::iterator iter = first_line.begin(); iter != first_line.end(); iter++)
	{
		wFile.open(iter->second.second);
		if (wFile.fail())
			throw std::out_of_range("Error, Fail to set HTML file");
		wFile << "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n	<title>";
		wFile << iter->first << " " << iter->second.first;
		wFile << "</title>\r\n</head>\r\n<body>\r\n	<h1>";
		wFile << iter->first << " " << iter->second.first;
		wFile << "</h1>\r\n</body>\r\n</html>\r\n";
		wFile.close();
	}
	wFile.open("./www/autoindex.html");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set HTML file");
	wFile << "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n	<style>\r\n		body {\r\n			padding: 30px;\r\n		}\r\n		h1, h2 {\r\n			font-weight: 400;\r\n			margin: 0;\r\n		}\r\n		h1 > span {\r\n			font-weight: 900;\r\n		}\r\n		ul > li {\r\n			font-size: 20px;\r\n		}\r\n	</style>\r\n	<meta charset=\"UTF-8\">\r\n	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n	<title>Webserv index of $1</title>\r\n</head>\r\n<body>\r\n	<h1>Webserv index of <span>$1</span></h1>\r\n	<hr>\r\n	<ul>\r\n		<pre>$2</pre>\r\n	</ul>\r\n	<hr>\r\n</body>\r\n</html>\r\n";
	wFile.close();

	wFile.open("./www/index.html");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set HTML file");
	wFile << "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Welcome to Webserv!</title>\r\n<style>\r\nhtml { color-scheme: light dark; }\r\nbody { width: 35em; margin: 0 auto;\r\nfont-family: Tahoma, Verdana, Arial, sans-serif; }\r\nh2 { color:darksalmon; }\r\n</style>\r\n</head>\r\n<body>\r\n<h1>Welcome to Webserv!</h1>\r\n<h2>First index page</h2>\r\n<p>If you see this page, the Webserv is successfully installed and\r\nworking. Further configuration is required.</p>\r\n\r\n<p>For online documentation and support please refer to\r\n<a href=\"http://nginx.org/\">nginx.org</a>.<br/>\r\nCommercial support is available at\r\n<a href=\"http://nginx.com/\">nginx.com</a>.</p>\r\n\r\n<p><em>Thank you for using webserv.</em></p>\r\n\r\n<hr>\r\n<p>1. Receive data with <em>\"GET\"</em> method</p>\r\n<p>-> Data that may be exposed as URL</p>\r\n<form action=\"./cgi-bin/get_name.php\" method=\"get\">\r\n	First Name: <input type = \"text\" name = \"first_name\"> <br />\r\n	Last Name: <input type = \"text\" name = \"last_name\" />\r\n	<input type = \"submit\" value = \"Submit\" />\r\n</form>\r\n\r\n<hr>\r\n<p>2. Receive data with <em>\"POST\"</em> method</p>\r\n<p>-> Data that should not be exposed as URL</p>\r\n<form action=\"./cgi-bin/get_profile.php\" method=\"post\">\r\n	ID : <input type = \"text\" name = \"ID\"> <br />\r\n	PW : <input type = \"text\" name = \"PW\" />\r\n	<input type = \"submit\" value = \"Submit\" />\r\n</form>\r\n\r\n<hr>\r\n<p>3. Uploade File</p>\r\n<form action=\"/cgi-bin/upload.php\" method=\"post\" enctype=\"multipart/form-data\">\r\n	Select image to upload:\r\n	<input type=\"file\" name=\"fileToUpload\" id=\"fileToUpload\">\r\n	<input type=\"submit\" value=\"Upload Image\" name=\"submit\">\r\n</form>\r\n\r\n</body>\r\n</html>\r\n";
	wFile.close();
}

// static void set_php_file()
// {
	
// }

void set_default_files(std::map<int, std::pair<std::string, std::string> >& first_line)
{
	set_html_file(first_line);
	// set_php_file();
}