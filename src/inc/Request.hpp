# pragma once

# include "Common.hpp"
# define SP ' '
# define DEFAULT_HTTP_PORT 80
# define DEFAULT_HTTPS_PORT 443
class Request
{
private:
	std::vector<char> _request;
	std::vector<char> _body;
	std::string _method;//!
	std::string _request_str;
	std::string _uri;//!
	std::string _protocol;//!
	std::map<std::string, std::string>_headers;//!
	// std::string _body;
	std::vector<std::string>_lines;
	std::string _request_line;
	int _index_aux;
	bool _valid;
	int _error_code;//!
	std::string _help_message;//!
	std::vector<std::string> _accept_method;
	int	_port;//!					// unsigned int?
	std::string _host;//!			// server_name???


	void manage_request(int socket_fd);
	void parse_request(const char *buffer);
	void get_lines(const std::string &request);
	void get_lines(std::vector<char> &request);

	void read_request_lines(std::vector<char> &request);
	void read_request_lines();
	void extract_request_line();

	bool check_number_elements_request_line(std::vector<std::string> result);
	bool check_any_valid_line();
	bool check_request_line();
	bool check_method();
	bool check_spaces_at_beginning();
	bool check_uri();
	bool check_protocol();
	void set_host_and_port(std::string &host_line_value);

	bool read_headers_lines();
	
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

	//  -----   SETTERS   -----  //
	bool set_validity(int error_code);
	bool set_validity(int error_code, std::string help_message);

	//  -----   GETTERS   -----  //
	std::vector<char> get_full_request();
	std::vector<char> get_body();
	std::string get_request_str();
	std::string get_method();
	std::string get_uri();
	std::string get_protocol();
	std::string get_request_line();
	std::string get_help_message();
	std::string get_host();
	std::vector<std::string> get_accept_method();
	int get_error_code();
	int get_port();
	bool get_validity();
	std::map<std::string, std::string> get_headers();






	// Para debug //
	void check_lines(std::vector<std::string> lines);
	void find_CRLF(const std::string &str);
	void find_CRLF(const std::vector<char> &request);
	void check_vector(const std::vector<char> &request);

	Location    compareUri(const std::vector<Location> &, const std::string &);
	bool		compareListen(std::vector<struct sockaddr_in> , int);
	bool		compareRequest(Server &);
	Response	&request_resolution(std::vector<Server> &);
};	


