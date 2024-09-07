# pragma once

# include "Common.hpp"
# define SP ' '
class Request
{
private:
	std::vector<char> _request;
	std::vector<char> _body;
	std::string _method;
	std::string _request_str;
	std::string _uri;
	std::string _protocol;
	std::map<std::string, std::string>_headers;
	// std::string _body;
	std::vector<std::string>_lines;
	std::string _request_line;
	int _index_aux;
	bool _valid;
	int _error_code;
	std::vector<std::string> _accept_metod;

public:
	bool debug;
	Request(void);
	Request(char *buffer);
	/* Request(std::string method, 
			std::string uri,
			std::string protocol,
			std::map<std::string, std::string> headers,
			std::string body); */
	Request(Request const &copy);
	Request & operator=(Request const & rhs);
	~Request();

	void manage_request(int socket_fd);
	void parse_request(const char *buffer);
	void get_lines(const std::string &request);
	void get_lines(std::vector<char> &request);

	void extract_request_lines(std::vector<char> &request);
	void extract_first_line();

	void check_any_valid_line();
	void check_request_line();
	
	bool get_validity();
	// Para debug //
	void check_lines(std::vector<std::string> lines);
	void find_CRLF(const std::string &str);
	void find_CRLF(const std::vector<char> &request);
	void check_vector(const std::vector<char> &request);
};


