# include "../inc/Webserver.hpp"

Request::Request() : _method(""), _uri(""), _protocol(""), _host(""), _port(0),
					_body(), _help_message(), _valid(true), _error_code(0),
					_headers(), _params(),
					_request(), _accept_method(), _request_line(""), _lines()
{}

/* Request::Request (std::string method, std::string uri, std::string protocol, 
					std::map<std::string, std::string> headers, std::string body):
				_method(method), _uri(uri), _protocol(protocol), _headers(headers), _body()
{
	// comprobar si tb vale cuando hay memoria reservada para headers, creo que sí.
} */

Request::Request(int i, int fd) : _fd_socket(fd), _pos_socket(i), _method(""), _uri(""), _protocol(""), _host(""), _port(0),
						  _body(), _help_message(), _valid(true), _error_code(0), _headers(),
						  _params(), _request(), _accept_method(), _request_line(""), _lines()
{
	std::cout << "Constructor con índice y fd llamado. Valor de i: " << i
				<<" valor de fd: " << fd << "\n";

}

Request::Request(int i, int fd, std::vector<char> request_accumulator) : _fd_socket(fd), _pos_socket(i), 
						  _req_uccumulator(request_accumulator), 
						  _method(""), _uri(""), _protocol(""), _host(""), _port(0),
						  _body(), _help_message(), _valid(true), _error_code(0), _headers(),
						  _params(), _request(), _accept_method(), _request_line(""), _lines()
{
	std::cout << "Constructor con índice, fd y acumulador llamado. Valor de i: " << i
				<<" valor de fd: " << fd << "\n";
	for (size_t i = 0; i < _req_uccumulator.size(); i++)
		std::cout << _req_uccumulator[i];
	std::cout << "\nFinal de la request\n" << std::endl;
	
}

Request::Request(int i, int fd, std::vector<char> request_accumulator, int type) : _fd_socket(fd), _pos_socket(i), 
						  _req_uccumulator(request_accumulator), 
						  _method(""), _uri(""), _protocol(""), _host(""), _port(0),
						  _body(), _help_message(), _valid(true), _error_code(0), _headers(),
						  _params(), _request(), _accept_method(), _request_line(""), _lines(), _type(type)
{
	std::cout << "Constructor útlimo guay\n";
	switch (_type)
	{
	case COMPLETE_REQUEST:
		manage_complete_request();
		break;
	case REQUEST_WITH_BODY:
		manage_request_with_body();
		break;
	case CHUNKED_REQUEST:
		manage_request_chunked();
		break;
	}
	
	
}

Request::Request(char *buffer) : _method(""), _uri(""), _protocol(""), _host(""), _port(0),
								_body(), _help_message(), _valid(true), _error_code(0),
								_headers(), _params(),
								_request(), _accept_method(), _request_line(""), _lines() 
{
	debug = true;
	if (debug == true)
	{
		std::cout << "\nConstructor BUFFER" << std::endl;
		_accept_method.push_back("GET");
		// _accept_metod.push_back("POST");
		_accept_method.push_back("DELETE");
	}
	this->_request.insert(_request.end(), buffer, buffer + std::strlen(buffer));
	// std::string aux(_request.begin(), _request.end());
	// _request_str = aux;
	read_request_lines();	
	check_any_valid_line();
	extract_request_line();
	check_request_line();
	read_headers_lines();
	if (debug == true){print_request_complete_info();}
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
			if (it != _request.end() 
				&& *it == '\r' && (it + 1) != _request.end() 
				&& *(it + 1) == '\n')
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
	if (check_number_elements_request_line(result))
	{
		_method = result[0];
		_uri = result[1];
		_protocol = result[2];
	}
	get_params_from_uri();
}

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

bool Request::read_headers_lines()
{
	std::string key;
	std::string value;
	size_t colon_position;

	if (_valid == false)
		return _valid;
	if (_lines.size() <= 1)
		return _valid;

	for (size_t i = 1; i < _lines.size(); i++)
	{
		colon_position = _lines[i].find(":");
		if (colon_position == _lines[i].npos || 
			_lines[i].find(" :") != _lines[i].npos)
			return set_validity(BAD_REQUEST);
		key = _lines[i].substr(0, colon_position);
		value = _lines[i].substr(colon_position + 1);
		_headers[key] = value;
		if (key == "Host")
			set_host_and_port(value);
	}
	return _valid;
}


bool Request::check_number_elements_request_line(std::vector<std::string> result)
{
	if (result.size() != 3)
		return set_validity(BAD_REQUEST);
	// if(debug == true){std::cout << "La línea tiene la cantidad  de elementos adecuada" << std::endl;}
	return true; 
}

void Request::get_params_from_uri()
{
	size_t question_mark_pos;
	question_mark_pos = _uri.find("?");

	if (question_mark_pos != _uri.npos)
	{
		std::string params_raw = _uri.substr(question_mark_pos + 1);
		_uri = _uri.substr(0, question_mark_pos);
		split_params(params_raw);
	}
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
	for (i = 0; i < _accept_method.size(); i++)
	{
		if (_accept_method[i] == _method)
			break;
	}
		// if (debug == true){std::cout << "i: " << i << std::endl;}
	if (i == _accept_method.size())
		return set_validity(METHOD_NOT_ALLOWED);
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

bool Request::check_protocol()
{
	if (_valid == false)
		return false;
	size_t bar_positition = _protocol.find("/");
	if (_protocol.find("/") == _protocol.npos)
		return set_validity(BAD_REQUEST);
	std::string protocol = _protocol.substr(0, bar_positition);
	std::string version = _protocol.substr(bar_positition + 1);
	// if (debug == true){std::cout << "Protocol: " << protocol << std::endl;
	// std::cout << "Version: " << version << std::endl;}
	if (protocol != "HTTP")
		return set_validity(BAD_REQUEST);
	if (version == "1.0")
		return set_validity(HTTP_VERSION_NOT_SUPPORTED);
	if (version != "1.1")
		return set_validity(BAD_REQUEST);
	return true;
}


void Request::set_host_and_port(std::string &host_line_value)
{
	size_t colon_position;

	colon_position = host_line_value.find(":");
	if (colon_position == host_line_value.npos)
	{
		_host = host_line_value;
		_port = DEFAULT_HTTP_PORT;
	}
	else
	{
		_host = host_line_value.substr(0, colon_position);
		_port = atoi(host_line_value.substr(colon_position + 1).c_str());
	}
}


void Request::split_params(std::string &params_raw)
{
	std::string::size_type start = 0;
	std::string::size_type end;
	std::string aux;
	std::vector<std::string> params_unchecked;

	end = params_raw.find("&");
	while (end != std::string::npos)
	{
		aux = params_raw.substr(start, end - start);
		params_unchecked.push_back(aux);
		start = end + 1;
		end = params_raw.find("&", start);
	}
	if (start < params_raw.length())
	{
		params_unchecked.push_back(params_raw.substr(start));
	}
	/* if (debug == true)
	{
		std::cout << "PARAMETROS EN VECTOR: " << std::endl;
		for (size_t i = 0; i < params_unchecked.size(); i++)
			std::cout << params_unchecked[i] << std::endl;
	} */
	check_and_set_params(params_unchecked);

}

bool Request::check_and_set_params(std::vector<std::string> params_unchecked)
{
	std::string key;
	std::string value;
	size_t equal_pos;

	for (size_t i = 0; i < params_unchecked.size(); i++)
	{
		if (params_unchecked[i].find("=") == std::string::npos ||
			params_unchecked[i].find(" =") != std::string::npos ||
			params_unchecked[i].find("= ") != std::string::npos)
		{
			_valid = false;
			_error_code = BAD_REQUEST;
			_help_message = "The query parameters are malformed or invalid.";
			return false;
		}
		else
		{
			equal_pos = params_unchecked[i].find("=");
			key = params_unchecked[i].substr(0, equal_pos);
			value = params_unchecked[i].substr(equal_pos + 1);
			_params[key] = value;
		}
	}
/* 	 if (debug == true)
	{
		std::cout << "Parametro  =  Valor"  << std::endl;
		for (std::map<std::string, std::string>::iterator it = _params.begin(); it != _params.end(); ++it)
			std::cout << it->first << " = " << it->second << std::endl;
	} */
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

/* void Request::get_lines(const std::string &request)
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
} */


// --------------------  GETTERS  -------------------- //
std::vector<char> Request::get_full_request()
{
	return (this->_request);
}
std::vector<char> Request::get_body()
{
	return (this->_body);
}
/* std::string Request::get_request_str()
{
	return (this->_request_str);
} */
std::string Request::get_method()
{
	return (this->_method);
}
std::string Request::get_uri()
{
	return (this->_uri);
}
std::string Request::get_protocol()
{
	return (this->_protocol);
}
std::string Request::get_request_line()
{
	return (this->_request_line);
}
std::string Request::get_help_message()
{
	return (this->_help_message);
}
std::string Request::get_host()
{
	return (this->_host);
}
std::vector<std::string> Request::get_accept_method()
{
	return (this->_accept_method);
}
int Request::get_error_code()
{
	return (this->_error_code);
}
int Request::get_port()
{
	return (this->_port);
}
bool Request::get_validity()
{
	return (this->_valid);
}
std::map<std::string, std::string> Request::get_headers()
{
	return _headers;
}
std::map<std::string, std::string> Request::get_params()
{
	return _params;
}
int Request::get_current_status()
{
	return _status;
}

// --------------------  SETTERS  -------------------- //
bool Request::set_validity(int error_code)
{
	_valid = false;
	_error_code = error_code;
	return _valid;
}
bool Request::set_validity(int error_code, std::string help_message)
{
	_valid = false;
	_error_code = error_code;
	_help_message = help_message;
	return _valid;
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

void Request::print_request_complete_info()
{
	if (_valid == true)
	{
	// Request line:
	std::cout << "\n------   REQUEST COMPLETE INFO:   -----\n\n--  Request Line:  --\n";
		// Method
		std::cout << "- Method: " << _method << "\n";
		// URI
		std::cout << "- URI: " << _uri << "\n";
		// Protocol
		std::cout << "- Protocol: " << _protocol << "\n";
		// Params
		std::cout << "- Parámetros: -\n - Parametro  =  Valor -\n";
		for (std::map<std::string, std::string>::iterator it = _params.begin(); it != _params.end(); ++it)
			std::cout << it->first << " = " << it->second << "\n";
	//Headers:
	std::cout << "\n--  Headers:  --\n";
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		std::cout << it->first << " = " << it->second << "\n";
	//Body:
	std::cout << "\n--  Body  --\n";
	for (size_t i = 0; i < _body.size(); i++)
	{
		std::cout << _body[i];
	}
	std::cout << std::endl;
	}
	else
	{
	std::cout << "Request no válida\n" 
		<< "validez: " << _valid 
		<< "\nCódigo error: " << _error_code 
		<< "Mensaje ayuda: " << _help_message << std::endl;
	}

}

// if (debug == true){std::cout << "" << std::endl;}




void Request::manage_complete_request()
{
	std::cout << "Request completa" << std::endl;
}
void Request::manage_request_with_body()
{
	std::cout << "Request con body" << std::endl;
}
void Request::manage_request_chunked()
{
	std::cout << "Request chunked" << std::endl;
}


//añadidas por Lyudmyla
//va despues de recibir el header (listen es siempre de server)
bool    Request::compareListen(std::vector<struct sockaddr_in> listen, int port)
{
    if (!listen.empty())
    {
        for (int i = 0; i < listen.size(); i++)
        {
            if (ntohs(listen[i].sin_port) == port)
                return (true);
        }
    }
    return (false);
}

Location    Request::compareUri(const std::vector<Location> &location, const std::string &uri)
{
    std::string uri_location;
    std::size_t found;

    for (int i = 0; i < location.size(); i++)
    {
        uri_location = location[i].getUri();
        found = uri.find(location[i].getUri());
        if (found == 0)
            return (location[i]);
    }
    return Location();
}

bool     Request::compareMethod(Server &server)
{
    if (this->_method == "GET" && server.getAcceptMethod().get == 0)
            return (false);
    if (this->_method == "POST" && server.getAcceptMethod().post == 0)
            return (false);
    if (this->_method == "DELETE" && server.getAcceptMethod().del == 0)
            return (false);
    return (true);
}
//esta funcion va despues de parsear la request line
int    Request::check_request_line(std::vector<Server> &server)
{
	std::vector<Server>::iterator it_serv;
	conf_loc = Location();

	for (it_serv = server.begin(); it_serv != server.end(); ++it_serv)//buscar si hay configuracion para este server name
    {
        if (this->_host == it_serv->getServerName())
            break;
    }
	if (it_serv != server.end())///si hay este server name
	{
		if (!compareListen(it_serv->getListen(), this->_port))
		{
			this->_error_code = NOT_FOUND;
        	return (0);
		}
		if (compareMethod(*it_serv))//comprobar em metodo y si el puerto por el que habla el cliente y el puerto de conf coinciden
        {
			if (!it_serv->getLocation().empty())//si el server no tiene location comprobamos la uri con root
				conf_loc = compareUri(it_serv->getLocation(), this->_uri);//buscamos si hay uri que esta pidiendo el cliente
			if (conf_loc.getBodySize() == -1)//no existe la location
			{
				if (this->_uri.find(it_serv->getRoot()) != 0)
				{
					this->client_max_body_size = it_serv->getBodySize();
					conf_serv = *it_serv;
					return (1);
				}
			}
			else
			{
				this->client_max_body_size = conf_loc.getBodySize();
				conf_serv = *it_serv;
				return (1);
			}
		}
    }
	this->_error_code = NOT_FOUND;
	return (0);
}

void  Request::last_conection_time()
{
      conecction_time = time(NULL);
}

time_t        Request::get_time()
{
      return (conecction_time);
}

void  Request::set_error_code(int error_code)
{
      this->_error_code = error_code;
}

void Request::manage_request_chunked()
{
      std::cout << "Request chunked" << std::endl;
}

Location	Request::getLoc() const
{
	return (this->conf_loc);
}

Server	Request::getServ() const
{
	return (this->conf_serv);
}
