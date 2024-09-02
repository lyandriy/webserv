# include "../inc/Request.hpp"

Request::Request() : _request(), _request_str(""), _method(""),  _uri(""), _protocol(""), _headers(), _body("")
{}

Request::Request (std::string method, std::string uri, std::string protocol, 
					std::map<std::string, std::string> headers, std::string body):
				_method(method), _uri(uri), _protocol(protocol), _headers(headers), _body(body)
{
	// comprobar si tb vale cuando hay memoria reservada para headers, creo que sí.
}

Request::Request(char *buffer) : _method(""), _uri(""), _protocol(""), _headers(), _body(""), _lines()
{
	std::cout << "Constructor BUFFER" << std::endl;
	_request.insert(_request.end(), buffer, buffer + std::strlen(buffer));
	std::string aux(_request.begin(), _request.end());
	_request_str = aux;
	// std::cout << "--->>>   Esta es la request en bruto:   <<<---\n" << _request_str << std::endl;
	// this->find_CRLF(_request_str);
	// std::cout << "\n\nEstas son las líneas de la request:\n" << std::endl;
	get_lines(_request_str);
	// this->check_lines(_lines);
	check_vector(_request);
}

Request::Request(Request const &copy)
{
	this->_method = copy._method;
	this->_uri = copy._uri;
	this->_protocol = copy._protocol;
	this->_body = copy._body;
	this->_headers = copy._headers;
}

Request& Request::operator=(Request const & other)
{
	if (this != &other)
	{
		this->_method = other._method;
		this->_uri = other._uri;
		this->_protocol = other._protocol;
		this->_body = other._body;
		this->_headers = other._headers;
	}
	return *this;
}

Request::~Request()
{}

void Request::parse_request(const char *buffer)
{
	(void)buffer;

}

void Request::manage_request(int socket_fd)
{
	(void)socket_fd;
}

void Request::get_lines(const std::string &request)
{
	std::string::size_type start = 0;
	std::string::size_type end;
	std::string line;

	end = request.find("\r\n", start);
	while (end != std::string::npos)
	{
		line = request.substr(start, end - start);
		_lines.push_back(line);
		start = end + 2;
		end = request.find("\r\n", start);
	}
	if (start < request.length())
	{
		_lines.push_back(request.substr(start));
	}
		std::cout << "WOLOLOOOOOOO!!!" << std::endl;
}

void get_lines(const std::vector<char> &request)
{
	(void)request;
}


// --------------------  DEBUG  -------------------- //
void Request::check_lines(std::vector<std::string> lines)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::cout << lines[i] << "\n";
	}
	std::cout << std::endl;
	
}

void Request::find_CRLF(const std::string &str)
{
	size_t position;
	position = str.find("\r\n", 15);
	std::cout << "\\r\\n encontrado en psosición " << position << std::endl;
}

