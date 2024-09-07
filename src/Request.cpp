# include "../inc/Request.hpp"

Request::Request() : _request(), _body(), _method(""), _request_str(""), _uri(""), _protocol(""),
					_headers(), _lines(), _request_line(), _index_aux(0), _valid(true), _error_code(0)
{}

/* Request::Request (std::string method, std::string uri, std::string protocol, 
					std::map<std::string, std::string> headers, std::string body):
				_method(method), _uri(uri), _protocol(protocol), _headers(headers), _body()
{
	// comprobar si tb vale cuando hay memoria reservada para headers, creo que sí.
} */

Request::Request(char *buffer) : _body(), _method(""), _request_str(""), _uri(""), _protocol(""), 
								_headers(), _lines(), _request_line(""), _index_aux(0), _valid(true), _error_code(0) 
{
	debug = true;
	if (debug == true)
	{
		_accept_metod.push_back("GET");
		_accept_metod.push_back("POST");
		_accept_metod.push_back("DELETE");
	}
	std::cout << "\nConstructor BUFFER" << std::endl;
	this->_request.insert(_request.end(), buffer, buffer + std::strlen(buffer));
	std::string aux(_request.begin(), _request.end());
	_request_str = aux;
	extract_request_lines(_request);
	check_any_valid_line();
	// check_lines(_lines);
	// check_vector(_body);
	extract_first_line();
	check_request_line();
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

void Request::check_request_line()
{
	size_t i = 0;
	if (this->_valid == false)
		return;
	if (this->_lines[0][0] == SP)
	{
		//verifica que la request no empiece con un espacio, no está permitido
		this->_valid = false;
		this->_error_code = 400;
	}
	// chequear que el método está en los válidos del servidor
	// para eso hay que saber a qué servidor va dirigido ¿y el puerto?
	// revisar el campo location los métodos permitidos
	//este código verifica si el método está en los permitidos (sin buscar el location)
	// si recorre todos los métodos permitidos y no ha encontrado que sea válido
	//establece la request como inválida y establece código de error 403
//Proceso lógico:
	//Llega request
	//verificar bloque server
	//Determinar bloquer locatio correspondiente
	//Verificar métodos dentro del location
	for (i = 0; i < _accept_metod.size(); i++)
	{
		if (_accept_metod[i] == _method)
		{
			break;
		}
	}
	if (i == _accept_metod.size())
	{
		_valid = false;
		_error_code = 405; //Method Not Allowed
	}
}

void Request::check_any_valid_line()
{
	if (this->_lines.empty())
	{
		this->_valid = false;
		this->_error_code = 400;
	}
}

void Request::extract_first_line()
{
	if (this->_valid == false)
		return;
	std::string first_line = _lines[0];
	std::vector<std::string> result;
	std::string aux;
	std::string::iterator it = first_line.begin();

	while (it != first_line.end())
	{
		if (*it == SP)
		{
			if (!aux.empty())
			{
				result.push_back(aux);
				aux.clear();
			}
		}
		else
		{
			aux += *it;
		}
		it++;
	}
	if (!aux.empty())
	{
		result.push_back(aux);
	}

	//para debug
	std::cout << "Estos son los componentes de la primera línea:\n";
	for (size_t i = 0; i < result.size(); i++)
	{
		std::cout << result[i] << "\n";
	}
	std::cout << std::endl;
	
}

void Request::extract_request_lines(std::vector<char> &request)
{
	std::vector<char>::iterator it = request.begin();
	std::vector<char>::iterator line_start = it;
	while (it != request.end())
	{
		if (*it == '\r' && (it + 1) != request.end() && *(it + 1) == '\n')
		{
			std::string line(line_start, it);

			_lines.push_back(line);
			it += 2;
			line_start = it;
			if (it != request.end() && *it == '\r' && (it + 1) != request.end() && *(it + 1) == '\n')
			{
				it += 2;
				if (it != request.end())
				{
					_body = std::vector<char> (it, request.end());
				}
			}
		}
		else
		{
			it++;
		}
	}
}

bool Request::get_validity()
{
	return _valid;
}

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
}

void get_lines(std::vector<char> &request)
{
	std::vector<char>::iterator begin = request.begin();
	(void)begin;
}


// --------------------  DEBUG  -------------------- //

void Request::check_lines(std::vector<std::string> lines)
{
	std::cout << "\nLíneas antes del body (" << lines.size() << "):\n";
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

void Request::check_vector(const std::vector<char> &request)
{
	std::cout << "Chequeo del vector:\n";
	for (size_t i = 0; i < request.size(); i++)
	{
		std::cout << request[i];
	}
	std::cout << std::endl;
}


