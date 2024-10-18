# include "../inc/Webserver.hpp"

Request::Request(int free_pfd, int new_sock) : _fd_socket(free_pfd), _pos_socket(new_sock), _req_accumulator(),
											_method(""), _protocol(""), _host(""), _port(0),
											_body(), _help_message(), _valid(true), _error_code(0), _headers(),
											_params(), _request(), _accept_method(), _request_line(""), _lines(),
											_type(0), _status(1)
{
	debug = true;
}

Request::Request() : _method(""), _uri(""), _protocol(""), _host(""), _port(0),
					_body(), _help_message(), _valid(true), _error_code(0),
					_headers(), _params(),
					_request(), _accept_method(), _request_line(""), _lines()
{}

Request::Request(int i, int fd, std::vector<char> request_accumulator, int type) : _fd_socket(fd), _pos_socket(i), 
						  _req_accumulator(request_accumulator), 
						  _method(""), _uri(""), _protocol(""), _host(""), _port(0),
						  _body(), _help_message(), _valid(true), _error_code(0), _headers(),
						  _params(), _request(), _accept_method(), _request_line(""), _lines()
{
	/*std::cout << "Constructor con índice, fd y acumulador llamado. Valor de i: " << i
				<<" valor de fd: " << fd << "\n";
	for (size_t i = 0; i < _req_accumulator.size(); i++)
		std::cout << _req_accumulator[i];
	std::cout << "\nFinal de la request\n" << std::endl;*/
	(void)type;
	
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
		this->_fd_socket = other._fd_socket;
		this->_pos_socket = other._pos_socket;
		this->_req_accumulator = other._req_accumulator;
		this->_method = other._method;
		this->_uri = other._uri;
		this->_protocol = other._protocol;
		this->_host = other._host;
		this->_port = other._port;
		this->_body = other._body;
		this->_help_message = other._help_message;
		this->_valid = other._valid;
		this->_error_code = other._error_code;
		this->_headers = other._headers;
		this->_params = other._params;
		this->_body_size = other._body_size;
		this->_request = other._request;
		this->_accept_method = other._accept_method;
		this->_request_line = other._request_line;
		this->_lines = other._lines;
		this->_type = other._type;
		this->_status = other._status;
		this->_CRLFx2_index = other._CRLFx2_index;
		this->conf_serv = other.conf_serv;
		this->conf_loc = other.conf_loc;
		this->server_body_size = other.server_body_size;
		this->conecction_time = other.conecction_time;
	}
	return *this;
}

Request::~Request()
{}


int Request::join_request(char *buffer, int read_size, std::vector<Server> &server)
{
	/*std::ofstream outfile("request.html");
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return INVALID_REQUEST; // Return an error if the file cannot be opened
    }

    // Write the contents of the buffer to the file, replacing \r and \n
    for (int i = 0; i < read_size; ++i) {
        if (buffer[i] == '\r') {
            outfile << "\\r"; // Replace \r with its visible representation
        } else if (buffer[i] == '\n') {
            outfile << "\\n"; // Replace \n with its visible representation
        } else {
            outfile << buffer[i]; // Write other characters as-is
        }
    }

    // Check if writing succeeded
    if (outfile.fail()) {
        std::cerr << "Error writing to file." << std::endl;
        outfile.close(); // Close the file before returning
        return INVALID_REQUEST; // Return an error if writing fails
    }

    // Close the file after writing
    outfile.close();*/
	//int server_body_size = 1024; // esto debe de venir de la configuración del server, pendiente pensar cómo hacer llegar este valor hasta aquí

	debug = true;

	switch (_status)
	{
	case INVALID_REQUEST:
		return INVALID_REQUEST;
		break;
	case INCOMPLETE_REQUEST:
		return manage_incomplete_request(buffer, read_size, server);
		break;
	case HEADERS_RECEIVED:
		return manage_headers_received(server);
		break;
	case REQUEST_WITH_BODY:
		return manage_request_with_body(buffer, read_size);
		break;
	case CHUNKED_REQUEST:
		return manage_chunked_request(buffer, read_size);
		break;
	case FULL_COMPLETE_REQUEST:
		return manage_full_complete_request(buffer, read_size);
		break;
	}
	return INCOMPLETE_REQUEST;
}

int	Request::manage_incomplete_request(char *buffer, int read_size, std::vector<Server> &server)
{
	_req_accumulator.insert(_req_accumulator.end(), buffer, buffer + read_size);
	if (search_double_CRLF() == false)
		return INCOMPLETE_REQUEST;
	else
	{
		_status = HEADERS_RECEIVED;
		return (manage_headers_received(server));
	}
	return INCOMPLETE_REQUEST;
}

bool Request::search_body_length_header()
{
	std::map<std::string, std::string>::iterator it;

	for(it = _headers.begin(); it != _headers.end(); it++)
	{
		if (it->first == "Content-Length")
		{
			_body_size = atoi(it->second.c_str());
			return true;
		}
	}
	return false;
}

bool Request::search_chunked_body()
{
	return false;
}

int	Request::manage_headers_received(std::vector<Server> &server)
{
	read_request_lines();
	if (check_any_valid_line() == false)
		return INVALID_REQUEST;
	// if (debug == true){std::cout << "VOY A LEER LOS HEADERS\n";}
	extract_request_line();
		// función Lyudmyla
	check_request_line(server);
	if (check_request_line() == false)
		return INVALID_REQUEST;
	if (read_headers_lines() == false)
		return INVALID_REQUEST;
	if (search_body_length_header() == true)
	{
		// esto empieza a crecer lo suficiente como para hacer pasarlo a manage_request_with_body ->NO, manage_request_w_body va a ser para almacenar los sucesivos buffers de la request con body en body  
		_status = REQUEST_WITH_BODY;
		// separar antes y después de CRLFx2
		if (_req_accumulator.size() - 4 == _CRLFx2_index - 1) // el CRLFx2 justo coincide en el final del vector
		{
			_body.clear();
		}
		else if (_req_accumulator.size() - 4 > _CRLFx2_index - 1) //hay cosas después del CRLFx2
		{
			std::vector<char> after_CRLFx2(_req_accumulator.begin() + _CRLFx2_index + 4, _req_accumulator.end());
			_body = after_CRLFx2;
		}
		// if (debug == true){std::cout << "ESTA ES LA LONGITUD DE BODY:" << _body_size << "\n";}
		// if (debug == true){std::cout << "ESTA ES LA LONGITUD DE BODY REAL:" << _body.size() << "\n";}
		//verificar longitud de body
		if (static_cast<int>(_body.size()) == _body_size) // teóricamente completa, no se deberían recibir más partes de esta request
		{
			_status = FULL_COMPLETE_REQUEST; 
		}
		if (static_cast<int>(_body.size()) > _body_size) // el body es más largo que el indicado en el header
		{
			set_validity(BAD_REQUEST, "The specified Content-Length does not match the actual size of the request body.");
		}
	}
	else if (search_chunked_body() == true)
	{
		if (_status == REQUEST_WITH_BODY)
		{
			set_validity(BAD_REQUEST, "Incompatible headers");
			return (INVALID_REQUEST);
		}
		else
		{
			_status = CHUNKED_REQUEST;
		}
	}
	else
	{
		_status = FULL_COMPLETE_REQUEST;
	}
	if (debug == true){print_full_info();}
	if (debug == true){std::cout << "Se han leído las líneas de los header\n" << std::endl;}
	if (debug == true){std::cout << "El estado es: " << _status << "\n" << std::endl;}

	return _status;
}

int	Request::manage_request_with_body(char *buffer, int read_size)
{
	//aquí ya ha sido procesado el header de Content-Length y extraído el valor del body size y posiblemente body tenga algún valor (o no si el último buffer coincidión con el CRLFx2)
	_body.insert(_body.end(), buffer, buffer + read_size);
	size_t body_len = _body.size();
	if (body_len == static_cast<size_t>(_body_size))
		_status = FULL_COMPLETE_REQUEST;
	if (body_len > static_cast<size_t>(_body_size))
	{
		set_validity(INVALID_REQUEST, "The specified Content-Length does not match the actual size of the request body.");
		_status = INVALID_REQUEST;
	}
	//si body_len < _body_size no hay que hacer nada, el status sigue siendo el mismo y hay que seguir leyendo más request
	return _status;
}

int	Request::manage_chunked_request(char *buffer, int read_size)
{
	(void)buffer;
	(void)read_size;
	return INVALID_REQUEST;
}

int	Request::manage_full_complete_request(char *buffer, int read_size)
{
	//set validity()
	(void)buffer;
	(void)read_size;
	return INVALID_REQUEST;
}

bool Request::search_double_CRLF()
{
	size_t request_len = _req_accumulator.size();
	if (request_len < 4)
	{
		_status = INCOMPLETE_REQUEST;
		return false;
	}
	
	for (size_t i = 0; i < request_len - 3; i++)
	{
		if (_req_accumulator[i] == '\r' && _req_accumulator[i+1] == '\n'
			 && _req_accumulator[i+2] == '\r' && _req_accumulator[i+3] == '\n')
		{
			_CRLFx2_index = i;
			_status = HEADERS_RECEIVED;
			return true;
		}
	}
	_status = INCOMPLETE_REQUEST;
	return false;
}

void Request::read_request_lines()
{
	std::vector<char>::iterator it = _req_accumulator.begin();
	std::vector<char>::iterator line_start = it;
	while (it != _req_accumulator.end())
	{
		if (*it == '\r' && (it + 1) != _req_accumulator.end() && *(it + 1) == '\n')
		{
			std::string line(line_start, it);

			_lines.push_back(line);
			it += 2; 
			line_start = it;
			if (it != _req_accumulator.end() 
				&& *it == '\r' && (it + 1) != _req_accumulator.end() 
				&& *(it + 1) == '\n')
			{
				it += 2;
				if (it != _req_accumulator.end())
				{
					_body = std::vector<char> (it, _req_accumulator.end());
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
	size_t i = 0;
	if (debug == true){std::cout << "Métodos aceptados: " << _accept_method.size() << "\n\n";}
	if (debug == true){_accept_method.push_back("GET");}
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
	return true;
}

// --------------------  GETTERS  -------------------- //
std::vector<char> Request::get_full_request()
{
	return (this->_request);
}
std::vector<char> Request::get_body()
{
	return (this->_body);
}

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

int	Request::get_fd_socket()
{
	return _fd_socket;
}

int	Request::get_pos_socket()
{
	return _pos_socket;
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

//añadidas por Lyudmyla
//va despues de recibir el header (listen es siempre de server)
bool    Request::compareListen(std::vector<struct sockaddr_in> listen)
{
    if (!listen.empty())
    {
        for (size_t i = 0; i < listen.size(); i++)
        {
            if (ntohs(listen[i].sin_port) == _port)
                return (true);
        }
    }
    return (false);
}

Location    Request::compareUri(const std::vector<Location> &location)
{
    std::string uri_location;
    std::size_t found;

    for (size_t i = 0; i < location.size(); i++)
    {
        uri_location = location[i].getUri();
        found = _uri.find(location[i].getUri());
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
		if (compareMethod(*it_serv))//comprobar em metodo y si el puerto por el que habla el cliente y el puerto de conf coinciden
        {
			if (!it_serv->getLocation().empty())//si el server no tiene location comprobamos la uri con root
				conf_loc = compareUri(it_serv->getLocation());//buscamos si hay uri que esta pidiendo el cliente
			if (conf_loc.getAutoindex() == -1)//no existe la location
			{
				if (!compareListen(it_serv->getListen()) && _body_size > it_serv->getBodySize())
				{
					this->_error_code = NOT_FOUND;
        			return (0);
				}
				if (this->_uri.find(it_serv->getRoot()) != 0)
				{
					this->server_body_size = it_serv->getBodySize();
					conf_serv = *it_serv;
					return (1);
				}
			}
			else
			{
				if (!compareListen(conf_loc.getListen()) && _body_size > conf_loc.getBodySize())
				{
					this->_error_code = NOT_FOUND;
        			return (0);
				}
				this->server_body_size = conf_loc.getBodySize();
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

Location	Request::getLoc() const
{
	return (this->conf_loc);
}

Server	Request::getServ() const
{
	return (this->conf_serv);
}

void	Request::set_current_status(int status)
{
	_status = status;
}


//--------------------PRINT REQUEST---------------------//

void Request::print_full_info() {
    std::cout << "-------- Request Info --------" << std::endl;

    // Información importante
    std::cout << "Socket FD: " << _fd_socket << std::endl;
    std::cout << "Socket Position: " << _pos_socket << std::endl;

    // Convertir el vector de chars a string para imprimir
    std::string req_accumulator_str(_req_accumulator.begin(), _req_accumulator.end());
    std::cout << "Request Accumulator: " << req_accumulator_str << std::endl;

    std::cout << "Method: " << get_method() << std::endl;
    std::cout << "URI: " << get_uri() << std::endl;
    std::cout << "Protocol: " << get_protocol() << std::endl;
    std::cout << "Host: " << get_host() << std::endl;
    std::cout << "Port: " << get_port() << std::endl;

    // Convertir el cuerpo de la request (vector de chars) a string para imprimir
    std::string body_str(_body.begin(), _body.end());
    std::cout << "Body: " << body_str << std::endl;

    std::cout << "Help Message: " << get_help_message() << std::endl;
    std::cout << "Valid: " << (get_validity() ? "true" : "false") << std::endl;
    std::cout << "Error Code: " << get_error_code() << std::endl;

    // Headers y parámetros
    std::cout << "Headers: " << std::endl;
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Params: " << std::endl;
    for (it = _params.begin(); it != _params.end(); ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    // Información sobre el cuerpo de la request
    std::cout << "Body Size: " << _body_size << std::endl;

    // Auxiliares
    std::string request_str(_request.begin(), _request.end());
    std::cout << "Request (as string): " << request_str << std::endl;

    std::vector<std::string>::const_iterator vec_it;
    std::cout << "Accepted Methods: ";
    std::vector<std::string> accept_methods = get_accept_method();
    for (vec_it = accept_methods.begin(); vec_it != accept_methods.end(); ++vec_it) {
        std::cout << *vec_it << " ";
    }
    std::cout << std::endl;

    std::cout << "Request Line: " << get_request_line() << std::endl;

    std::cout << "Lines: " << std::endl;
    for (vec_it = _lines.begin(); vec_it != _lines.end(); ++vec_it) {
        std::cout << "  " << *vec_it << std::endl;
    }

    std::cout << "Type: " << _type << std::endl;
    std::cout << "Status: " << _status << std::endl;
    std::cout << "CRLFx2 Index: " << _CRLFx2_index << std::endl;

    // Datos adicionales añadidos por Lyudmyla
    std::cout << "Server Config: " << std::endl;
    // Aquí puedes implementar la lógica para imprimir la configuración de `Server` y `Location` si tienes getters adecuados para ellos.
    std::cout << "Server Body Size: " << server_body_size << std::endl;
    std::cout << "Connection Time: " << conecction_time << std::endl;

    std::cout << "--------------------------------" << std::endl;
}

/*void	Request::ok_request()
{
	std::cout << "\033[35m" << " OKKKKKKKKKKKKKKKKK " <<  "\033[0m" << std::endl;
	_status = FULL_COMPLETE_REQUEST;
	_accept_method.push_back("GET");
	_host = "example.com";
	_body_size = 78;
	_error_code = 200;
}*/