# include "../inc/Request.hpp"

Request::Request() : _request(), _request_str(""), _method(""),  _uri(""), _protocol(""), _headers(), _body("")
{}

Request::Request (std::string method, std::string uri, std::string protocol, 
					std::map<std::string, std::string> headers, std::string body):
				_method(method), _uri(uri), _protocol(protocol), _headers(headers), _body(body)
{
	// comprobar si tb vale cuando hay memoria reservada para headers, creo que sí.
}

Request::Request(char *buffer) : _method(""), _uri(""), _protocol(""), _headers(), _body("")
{
	std::cout << "Constructor BUFFER" << std::endl;
	_request.insert(_request.end(), buffer, buffer + std::strlen(buffer));
	std::string aux(_request.begin(), _request.end());
	_request_str = aux;
	std::cout << "Esta es la request en bruto: " << _request_str << std::endl;
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
