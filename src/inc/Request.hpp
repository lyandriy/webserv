# pragma once

# include "../inc/Webserver.hpp"
# define SP ' '

# define DEFAULT_HTTP_PORT 80
# define DEFAULT_HTTPS_PORT 443


// --- STATUS --- //
# define EMPTY_REQUEST -1
# define INVALID_REQUEST  0
# define INCOMPLETE_REQUEST 1
# define HEADERS_RECEIVED 2
# define REQUEST_WITH_BODY 3
# define CHUNKED_REQUEST 4
# define FULL_COMPLETE_REQUEST 5

# define BODY_SIZE_BIGGER_THAN_SERVER_SUPPORTED 0

# define DOUBLE_CRLF "\r\n\r\n"

class Server;
class Location;

class Request
{
private:
	// Importantes //
	int					_fd_socket;
	int					_pos_socket;
	std::vector<char>	_req_accumulator;
	std::string 		_method;
	std::string 		_uri;
	std::string 		_protocol;
	std::string 		_host;			
	int					_port;			// unsigned int?//puerdo de request line
	std::vector<char>	_body;
	std::string			_help_message;
	bool				_valid;
	int					_error_code;
	std::map<std::string, std::string> _headers;//esta aqui el puerto de header
	std::map<std::string, std::string> _params;

	// Solo para request con body //
	int					_body_size;//del header

	// Auxiliares //
	std::vector<char> 			_request;
	std::vector<std::string> 	_accept_method;
	std::string 				_request_line;
	std::vector<std::string>	_lines;
	int							_type;
	int							_status;
	size_t						_CRLFx2_index;
	// std::string _request_str;  //solo lo conservo por si acaso, funciones que lo usan tb comentadas

	//añadidas por Lyudmyla
	Server	conf_serv;
	Location	conf_loc;
	int  server_body_size;
	time_t						conecction_time;


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

	bool search_double_CRLF();

	int	manage_incomplete_request(char *, int, std::vector<Server> &);
	int	manage_headers_received(std::vector<Server> &);
	int manage_request_with_body(char *, int);
	int manage_chunked_request(char *, int);
	int manage_full_complete_request(char *, int);

	bool search_body_length_header();
	bool search_chunked_body();

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

	int join_request(char *buffer);
	int join_request(char *buffer, int read_size, std::vector<Server> &server);


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
	int get_current_status();
	int	get_fd_socket();
	int	get_pos_socket();


	// Location    compareUri(const std::vector<Location> &, const std::string &);
	// bool		compareListen(std::vector<struct sockaddr_in> , int);
	// bool		compareRequest(Server &);
	// Response	request_resolution(std::vector<Server> &);

	//añadidas por Lyudmyla
	int    check_request_line(std::vector<Server> &);//!!!
	Location    compareUri(const std::vector<Location> &);
	bool		compareListen(std::vector<struct sockaddr_in>);//!!!
	bool		compareMethod(Server &);
	void	last_conection_time();
	void	set_error_code(int);
	void	set_current_status(int);
	Location	getLoc() const;
	Server	getServ() const;


	// Para debug //
	void check_lines(std::vector<std::string> lines);
	void find_CRLF(const std::string &str);
	void find_CRLF(const std::vector<char> &request);
	void check_vector(const std::vector<char> &request);
	void print_request_complete_info();
	time_t	get_time();
	void print_full_info();
	//void	ok_request();
};

