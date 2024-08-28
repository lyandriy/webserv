# pragma once

# include "Common.hpp"

class Request
{
private:
	std::vector<char> _request;
	std::string _request_str;
	std::string _method;
	std::string _uri;
	std::string _protocol;
	std::map<std::string, std::string>_headers;
	std::string _body;
public:
	Request(void);
	Request(char *buffer);
	Request(std::string method, 
			std::string uri,
			std::string protocol,
			std::map<std::string, std::string> headers,
			std::string body);
	Request(Request const &copy);
	Request & operator=(Request const & rhs);
	~Request();

	void manage_request(int socket_fd);
	void parse_request(const char *buffer);
};


