# pragma once

# include "Common.hpp"
# define SP ' '

# define DEFAULT_HTTP_PORT 80
# define DEFAULT_HTTPS_PORT 443

# define INCOMPLETE_REQUEST 0
# define COMPLETE_REQUEST 1
# define REQUEST_WITH_BODY 2
# define CHUNKED_REQUEST 3

class Request
{
private:
	// Importantes //
	int					_fd_socket;
	int					_pos_socket;
	std::vector<char>	_req_uccumulator;
	std::string 		_method;
	std::string 		_uri;
	std::string 		_protocol;
	std::string 		_host;			// server_name???
	int					_port;			// unsigned int?
	std::vector<char>	_body;
	std::string			_help_message;
	bool 				_valid;
	int 				_error_code;
	std::map<std::string, std::string> _headers;
	std::map<std::string, std::string> _params;

	// Auxiliares //
	std::vector<char> 			_request;
	std::vector<std::string> 	_accept_method;
	std::string 				_request_line;
	std::vector<std::string>	_lines;
	int							_type;
	// std::string _request_str;  //solo lo conservo por si acaso, funciones que lo usan tb comentadas


	void read_request_lines();
	bool check_any_valid_line();
	void extract_request_line();
	bool check_request_line();
	bool read_headers_lines();

	bool check_number_elements_request_line(std::vector<std::string> result);
	void get_params_from_uri();

	bool check_spaces_at_beginning();
	bool check_method();
	bool check_uri();
	bool check_protocol();

	void set_host_and_port(std::string &host_line_value);
	
	void split_params(std::string &params_raw);
	bool check_and_set_params(std::vector<std::string> params_unchecked);

	void manage_complete_request();
	void manage_request_with_body();
	void manage_request_chunked();


public:
	bool debug;
	Request(void);
	Request(int i, int fd);
	Request(int i, int fd, std::vector<char> request_accumulator);
	Request(int i, int fd, std::vector<char> request_accumulator, int type);
	Request(char *buffer);
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
	std::map<std::string, std::string> get_params();


	// Location    compareUri(const std::vector<Location> &, const std::string &);
	// bool		compareListen(std::vector<struct sockaddr_in> , int);
	// bool		compareRequest(Server &);
	// Response	request_resolution(std::vector<Server> &);



	// Para debug //
	void check_lines(std::vector<std::string> lines);
	void find_CRLF(const std::string &str);
	void find_CRLF(const std::vector<char> &request);
	void check_vector(const std::vector<char> &request);
	void print_request_complete_info();
};

void manage_complete_request(std::vector<char>request_accumulator);
void manage_request_with_body(std::vector<char>request_accumulator);
void manage_request_chunked(std::vector<char>request_accumulator);
