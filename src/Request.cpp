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
		// _accept_metod.push_back("POST");
		_accept_metod.push_back("DELETE");
	}
	std::cout << "\nConstructor BUFFER" << std::endl;
	this->_request.insert(_request.end(), buffer, buffer + std::strlen(buffer));
	std::string aux(_request.begin(), _request.end());
	_request_str = aux;
	read_request_lines();
	check_any_valid_line();
	extract_request_line();
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


bool Request::check_request_line()
{
	if (this->_valid == false)
		return false;
	check_spaces_at_beginning();
	check_method();
	check_uri();
	check_protocol();
	return true;
}

bool Request::check_protocol()
{
	if (_valid == false)
		return false;
	size_t bar_positition = _protocol.find("/");
	if (_protocol.find("/") == _protocol.npos)
		return set_validity(BAD_REQUEST);
	std::string protocol = _protocol.substr(0, bar_positition);
	std::string version = _protocol.substr(bar_positition + 1);
	if (debug == true){std::cout << "Protocol: " << protocol << std::endl;
	std::cout << "Version: " << version << std::endl;}
	if (protocol != "HTTP")
		return set_validity(BAD_REQUEST);
	if (version == "1.0")
		return set_validity(HTTP_VERSION_NOT_SUPPORTED);
	if (version != "1.1")
		return set_validity(BAD_REQUEST);
	return true;
}

bool Request::check_uri()  //falta implementar
{
	if (_valid == false)
		return false;
	// buscar uri en location
		// buscar coincidencia exacta
		// buscar coincidencia parcial
		// si no hay, retroceder un / y volver a comparar
	return true;
}

bool Request::check_spaces_at_beginning()
{
	if (_valid == false)
		return false;
	if (this->_lines[0][0] == SP)
		return set_validity(BAD_REQUEST);
	return true;
}

bool Request::check_method()
{
	if (_valid == false)
		return false;
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
	size_t i;
	for (i = 0; i < _accept_metod.size(); i++)
	{
		if (_accept_metod[i] == _method)
			break;
	}
		// if (debug == true){std::cout << "i: " << i << std::endl;}
	if (i == _accept_metod.size())
		return set_validity(METHOD_NOT_ALLOWED);
	return true;
}

bool Request::check_any_valid_line()
{
	if (this->_lines.empty())
		return set_validity(BAD_REQUEST);
	return true;
}


void Request::extract_request_line()
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

	if (debug == true)
	{
		std::cout << "Estos son los componentes de la primera línea:\n";
		for (size_t i = 0; i < result.size(); i++)
		{
			std::cout << result[i] << std::endl;
		}
	}
	if (check_number_elements_request_line(result))
	{
		_method = result[0];
		_uri = result[1];
		_protocol = result[2];
	}
}

bool Request::check_number_elements_request_line(std::vector<std::string> result)
{
	if (result.size() != 3)
		return set_validity(BAD_REQUEST);
	// if(debug == true){std::cout << "La línea tiene la cantidad  de elementos adecuada" << std::endl;}
	return true; 
}


/* void Request::read_request_lines(std::vector<char> &request)
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
} */

void Request::read_request_lines()
{
	std::vector<char>::iterator it = _request.begin();
	std::vector<char>::iterator line_start = it;
	while (it != _request.end())
	{
		if (*it == '\r' && (it + 1) != _request.end() && *(it + 1) == '\n')
		{
			std::string line(line_start, it);

			_lines.push_back(line);
			it += 2;
			line_start = it;
			if (it != _request.end() && *it == '\r' && (it + 1) != _request.end() && *(it + 1) == '\n')
			{
				it += 2;
				if (it != _request.end())
				{
					_body = std::vector<char> (it, _request.end());
				}
			}
		}
		else
		{
			it++;
		}
	}
}

bool Request::set_validity(int error_code)
{
	_valid = false;
	_error_code = error_code;
	return _valid;
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

		// if (debug == true){std::cout << "" << std::endl;}

