#include "error_page.hpp"

void set_error_page(void)
{
	std::map<int, std::string> status_code;
	status_code.insert(std::make_pair(100, "Continue"));						// 
	status_code.insert(std::make_pair(101, "Switching Protocols"));				// 
	status_code.insert(std::make_pair(200, "OK"));								// 정상
	status_code.insert(std::make_pair(201, "Created"));							// 
	status_code.insert(std::make_pair(202, "Accepted"));						// 
	status_code.insert(std::make_pair(203, "Non-Authoritative Information"));	// 
	status_code.insert(std::make_pair(204, "No Content"));						// 
	status_code.insert(std::make_pair(205, "Reset Content"));					// 
	status_code.insert(std::make_pair(206, "Partial Content"));					// 
	status_code.insert(std::make_pair(300, "Multiple Choices"));				// 
	status_code.insert(std::make_pair(301, "Moved Permanently"));				// 
	status_code.insert(std::make_pair(302, "Found"));							// 
	status_code.insert(std::make_pair(303, "See Other"));						// 
	status_code.insert(std::make_pair(304, "Not Modified"));					// 
	status_code.insert(std::make_pair(305, "Use Proxy"));						// 
	status_code.insert(std::make_pair(307, "Temporary Redirect"));				// 
	status_code.insert(std::make_pair(400, "Bad Request"));						// 
	status_code.insert(std::make_pair(401, "Unauthorized"));					// 
	status_code.insert(std::make_pair(402, "Payment Required"));				// 
	status_code.insert(std::make_pair(403, "Forbidden"));						// 
	status_code.insert(std::make_pair(404, "Not Found"));						// 
	status_code.insert(std::make_pair(405, "Method Not Allowed"));				// Method 권한이 없을 때
	status_code.insert(std::make_pair(406, "Not Acceptable"));					// 
	status_code.insert(std::make_pair(407, "Proxy Authentication Required"));	// 
	status_code.insert(std::make_pair(408, "Request Timeout"));					// 
	status_code.insert(std::make_pair(409, "Conflict"));						// 
	status_code.insert(std::make_pair(410, "Gone"));							// 해당 html 파일이 존재하지 않을 때
	status_code.insert(std::make_pair(411, "Length Required"));					// 
	status_code.insert(std::make_pair(412, "Precondition Failed"));				// 
	status_code.insert(std::make_pair(413, "Payload Too Large"));				// 
	status_code.insert(std::make_pair(414, "URI Too Long"));					// 
	status_code.insert(std::make_pair(415, "Unsupported Media Type"));			// 
	status_code.insert(std::make_pair(416, "Range Not Satisfiable"));			// 
	status_code.insert(std::make_pair(417, "Expectation Failed"));				// 
	status_code.insert(std::make_pair(426, "Upgrade Required"));				// 
	status_code.insert(std::make_pair(500, "Internal Server Error"));			// 
	status_code.insert(std::make_pair(501, "Not Implemented"));					// 
	status_code.insert(std::make_pair(502, "Bad Gateway"));						// 
	status_code.insert(std::make_pair(503, "Service Unavailable"));				// 
	status_code.insert(std::make_pair(504, "Gateway Timeout"));					// 
	status_code.insert(std::make_pair(505, "HTTP Version Not Supported"));		// HTTP version이 1.1이 아닐 때
}