#include "Config.hpp"
#include "Connect.hpp"

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
	wFile.open("./www/index.html");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set HTML file");
	wFile << "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Index page</title>\r\n</head>\r\n<body>\r\n<h1>This is Index page</h1>\r\n<h3>To test autoindex, Turn off the Index or put wrong Index on Configure file</h3>\r\n</body>\r\n</html>\r\n";
	wFile.close();

	wFile.open("./index.html");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set HTML file");

	wFile << "<!DOCTYPE html>\r\n<head>\r\n	<title>PHP Test</title>\r\n	<style>\r\n		body {margin : auto 40%;}\r\n		h1 {font-size : 50px;}\r\n	</style>\r\n</head>\r\n<body>\r\n	<h1> CGI Test</h1>\r\n	<h2>GET Test</h2>\r\n	<form action=\"/cgi-bin/get.php\" method=\"get\">\r\n		<p>Input 1 : <input type=\"text\" name=\"first\"> </p>\r\n		<p>Input 2 : <input type=\"text\" name=\"second\"> </p>\r\n		<input type=\"submit\" value=\"Submit\">\r\n	</form>\r\n	<h2>POST Test - text</h2>\r\n	<form action=\"/cgi-bin/post_text.php\" method=\"post\">\r\n		<p>Input 1 : <input type=\"text\" name=\"first\"> </p>\r\n		<p>Input 2 : <input type=\"text\" name=\"second\"> </p>\r\n		<input type=\"submit\" value=\"Submit\">\r\n	</form>\r\n	<h2>POST Test - file</h2>\r\n	<form action=\"/cgi-bin/post_file.php\" method=\"post\" enctype=\"multipart/form-data\">\r\n		<p>Image to upload : <input type=\"file\" name=\"fileToUpload\"> </p>\r\n	<input type=\"submit\" value=\"Submit\">\r\n	</form>\r\n</body>\r\n</html>";
	wFile.close();
	
	wFile.open("./test/index.html");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set HTML file");
	wFile << "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Index page</title>\r\n</head>\r\n<body>\r\n<h1>This is Index page</h1>\r\n<h3>To test autoindex, Turn off the Index or put wrong Index on Configure file</h3>\r\n</body>\r\n</html>\r\n";
	wFile.close();

	wFile.open("./cgi-bin/cgi_test.html");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set HTML file");
}

static void set_php_file()
{
	std::ofstream wFile;
	wFile.open("./cgi-bin/get.php");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set PHP file");
	wFile << "<?php\r\n$body = \"<!DOCTYPE html>\\n\";\r\n$body .= \"<head>\\n\";\r\n$body .= \"<title>PHP GET Test Result</title>\\n\";\r\n$body .= \"</head>\\n\";\r\n$body .= \"<body>\\n\";\r\n$body .= \"<h1>PHP GET Test Result</h1>\\n\";\r\nif (!isset($_REQUEST[\"first\"]))\r\n	$body .= \"No first input<p>\\n\";\r\nelse\r\n{\r\n	$body .= \"<p>First input : [ \";\r\n	$body .= $_REQUEST[\"first\"];\r\n	$body .= \" ]</p>\";\r\n}\r\nif (!isset($_REQUEST[\"first\"]))\r\n	$body .= \"No second input<p>\\n\";\r\nelse\r\n{\r\n	$body .= \"<p>Second input : [ \";\r\n	$body .= $_REQUEST[\"second\"];\r\n	$body .= \" ]</p>\";\r\n}\r\n$body .= \"</body>\\n\";\r\n$body .= \"</html>\\n\";\r\n$b_len = strlen($body);\r\nheader(\"Content-Length: $b_len\\r\\n\");\r\necho $body;\r\n?>";
	wFile.close();

	wFile.open("./cgi-bin/post_file.php");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set PHP file");
	wFile << "<?php\r\n$target_dir = \"uploads/\";\r\n$target_file = $target_dir . basename($_FILES[\"fileToUpload\"][\"name\"]);\r\n$uploadOk = 1;\r\n$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));\r\n\r\n$msg = \"\";\r\n\r\nif (file_exists($target_file))\r\n{\r\n	$msg .= \"[ERROR] file already exists<br>\";\r\n	$uploadOk = 0;\r\n}\r\n\r\nif ($uploadOk == 1)\r\n{\r\n	if (move_uploaded_file($_FILES[\"fileToUpload\"][\"tmp_name\"], $target_file))\r\n		$msg .=\"The file [\" . htmlspecialchars(basename($_FILES[\"fileToUpload\"][\"name\"])) . \"] has been uploaded.\";\r\n	else\r\n		$msg .= \"Sorry, your file was not uploaded\";\r\n}\r\nelse\r\n{\r\n	$msg .= \"Sorry, your file was not uploaded\";\r\n}\r\n\r\n$body = \"<!DOCTYPE html>\\n\";\r\n$body .= \"<head>\\n\";\r\n$body .= \"<title>PHP POST-file Test Result</title>\\n\";\r\n$body .= \"</head>\\n\";\r\n$body .= \"<body>\\n\";\r\n$body .= \"<h1>PHP POST-file Test Result</h1>\\n\";\r\n\r\n$body .= \"<p>\";\r\n$body .= $msg;\r\n$body .= \"</p>\";\r\n$body .= \"</body>\\n\";\r\n$body .= \"</html>\\n\";\r\n$b_len = strlen($body);\r\nheader(\"HTTP/1.1 200 OK\\r\\n\");\r\nheader(\"Content-Length: $b_len\\r\\n\");\r\necho $body;\r\n?>";
	wFile.close();
	
	wFile.open("./cgi-bin/post_text.php");
	if (wFile.fail())
		throw std::out_of_range("Error, Fail to set PHP file");
	wFile << "<?php\r\n$body = \"<!DOCTYPE html>\\n\";\r\n$body .= \"<head>\\n\";\r\n$body .= \"<title>PHP POST-text Test Result</title>\\n\";\r\n$body .= \"</head>\\n\";\r\n$body .= \"<body>\\n\";\r\n$body .= \"<h1>PHP POST Test - text Result</h1>\\n\";\r\nif (!isset($_REQUEST[\"first\"]))\r\n	$body .= \"No first input<p> \\n\";\r\nelse\r\n{\r\n	$body .= \"<p>First input : [ \";\r\n	$body .= $_REQUEST[\"first\"];\r\n	$body .= \" ]</p>\";\r\n}\r\nif (!isset($_REQUEST[\"first\"]))\r\n	$body .= \"No second input<p> \\n\";\r\nelse\r\n{\r\n	$body .= \"<p>Second input : [ \";\r\n	$body .= $_REQUEST[\"second\"];\r\n	$body .= \" ]</p>\";\r\n}\r\n$body .= \"</body>\\n\";\r\n$body .= \"</html>\\n\";\r\n$b_len = strlen($body);\r\nheader(\"HTTP/1.1 200 OK\\r\\n\");\r\nheader(\"Content-Length: $b_len\\r\\n\");\r\necho $body;\r\n?>";
	wFile.close();
}

void set_default_files(Connect& cn, std::map<int, std::pair<std::string, std::string> >& first_line)
{
	cn.default_error_page = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n	<title>Default Error Page</title>\r\n</head>\r\n<body>\r\n	<h1>This is Default error page</h1>\r\n</body>\r\n</html>";
	set_html_file(first_line);
	set_php_file();
}
