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
	std::vector<std::string>_lines;
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
	void get_lines(const std::string &_requests);

	// Para debug //
	void check_lines(std::vector<std::string> lines);
	void find_CRLF(const std::string &str);
};


