# include "../inc/Webserver.hpp"

Request::Request(int free_pfd, int new_sock) : _fd_socket(free_pfd), _pos_socket(new_sock), _req_accumulator(),
											_method(""), _protocol(""), _host(""), _port(0),
											_body(), _help_message(), _valid(true), _error_code(200), _headers(),
											_params(), _body_size(0), _chunks(), _accept_method(), _request_line(""), 
											_lines(), _type(0), _status(1), _multipart(false)
{

	conf_loc = Location();
	conf_serv = Server(-1);
	debug = true;
	_last_chunk_size = -1;
}

Request::Request() : _method(""), _uri(""), _protocol(""), _host(""), _port(0),
					_body(), _help_message(), _valid(true), _error_code(200),
					_headers(), _params(),
					_chunks(), _accept_method(), _request_line(""), _lines()
{}

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
		this->_chunks = other._chunks;
		this->_chunk_size = other._chunk_size;
		this->_accept_method = other._accept_method;
		this->_request_line = other._request_line;
		this->_lines = other._lines;
		this->_type = other._type;
		this->_status = other._status;
		this->_last_chunk_size = other._last_chunk_size;
		this->_CRLFx2_index = other._CRLFx2_index;
		this->conf_serv = other.conf_serv;
		this->conf_loc = other.conf_loc;
		this->server_body_size = other.server_body_size;
		this->conecction_time = other.conecction_time;
		this->upload_files = other.upload_files;
		this->_multipart = other._multipart;
		this->_boundary = other._boundary;
	}
	return *this;
}

Request::~Request()
{}


int Request::join_request(char *buffer, int read_size, std::vector<Server> &server)
{
	switch (_status)
	{
	case INVALID_REQUEST:
		return INVALID_REQUEST;
		break;
	case EMPTY_REQUEST:
	case INCOMPLETE_REQUEST:
		return manage_incomplete_request(buffer, read_size, server);
	case HEADERS_RECEIVED:
		return manage_headers_received(server);
	case REQUEST_WITH_BODY:
		return manage_request_with_body(buffer, read_size);
	case CHUNKED_REQUEST:
		return manage_chunked_request(buffer, read_size);
	case FULL_COMPLETE_REQUEST:
		return manage_full_complete_request(buffer, read_size);
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
	std::map<std::string, std::string>::iterator it;
	std::string chunked = "chunked";
	for(it = _headers.begin(); it != _headers.end(); it++)
	{
		if (it->first == "Transfer-Encoding")
		{
			if (spaces_trim(it->second) == chunked)
				return true;
		}
	}
	return false;
}

int	Request::manage_headers_received(std::vector<Server> &server)
{
	read_request_lines();
	if (check_any_valid_line() == false)
		return INVALID_REQUEST;
	extract_request_line();
	if (check_request_line() == false)
		return INVALID_REQUEST;
	if (read_headers_lines() == false)
		return INVALID_REQUEST;
	if (search_body_length_header() == true)
	{
		_status = REQUEST_WITH_BODY;
		split_at_CRLFx2();
		if (static_cast<int>(_body.size()) == _body_size)
		{
			_status = FULL_COMPLETE_REQUEST;
		}
		if (static_cast<int>(_body.size()) > _body_size)
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
		_status = CHUNKED_REQUEST;
		_body.clear();
		manage_possible_chunked_beggining();
	}
	else
	{
		_status = FULL_COMPLETE_REQUEST;
	}
	check_request_line(server);
	multipart();
	return _status;
}

void Request::split_at_CRLFx2()
{
	if (_req_accumulator.size() - 4 == _CRLFx2_index - 1)
	{
		_body.clear();
	}
	else if (_req_accumulator.size() - 4 > _CRLFx2_index - 1)
	{
		std::vector<char> after_CRLFx2(_req_accumulator.begin() + _CRLFx2_index + 4, _req_accumulator.end());
		if (_status == REQUEST_WITH_BODY)
			_body = after_CRLFx2;
		else if (_status == CHUNKED_REQUEST)
			_chunks = after_CRLFx2;
		else
			_body = after_CRLFx2;
	}
}

int Request::manage_possible_chunked_beggining()
{
	split_at_CRLFx2();
	std::pair<long, std::vector<char> > aux;
	std::vector<char>::iterator  it = _chunks.begin();
	size_t start = 0;
	size_t end = 0;
	size_t CRLF_count = 0;

	for (size_t i = 0; i < _chunks.size(); i++)
	{
		if (_chunks[i] == '\r' && i + 1 <= _chunks.size() && _chunks[i + 1] == '\n')
		{
			CRLF_count++;
			end = i;
			if (CRLF_count % 2 == 1)
			{
				std::string number_str(it + start, it + end);
				aux.first = std::strtol(number_str.c_str(), NULL, 16);
				start = i + 2;
			}
			if (CRLF_count % 2 == 0)
			{
				std::string text_str(it + start, it + end);
				if (aux.first != static_cast<long>(text_str.size()))
				{
					_status = INVALID_REQUEST;
					return (set_validity(BAD_REQUEST, "Chunk lentgh doesn't match"));
				}
				_body.insert(_body.end(), it + start, it + end);
				start = i + 2;
			}
			if (aux.first == 0)
			{
				_status = FULL_COMPLETE_REQUEST;
				return _status;
			}
		}
	}
		_last_chunk_size = (CRLF_count % 2 == 1) ? aux.first : -1;
	_chunks.erase(_chunks.begin(), _chunks.begin() + start);
	return CHUNKED_REQUEST;
}

int	Request::manage_request_with_body(char *buffer, int read_size)
{
	_body.insert(_body.end(), buffer, buffer + read_size);
	size_t body_len = _body.size();
	if (body_len == static_cast<size_t>(_body_size))
	{
		_status = FULL_COMPLETE_REQUEST;
		multipart();
	}
	if (body_len > static_cast<size_t>(_body_size))
	{
		set_validity(INVALID_REQUEST, "The specified Content-Length does not match the actual size of the request body.");
		_status = INVALID_REQUEST;
	}
	return _status;
}

int	Request::manage_chunked_request(char *buffer, int read_size)
{
	std::pair<long, std::vector<char> > aux;
	std::vector<char>::iterator  it = _chunks.begin();
	size_t start = 0;
	size_t end = 0;
	size_t CRLF_count;
	(void)read_size;
	
	_chunks.insert(_chunks.end(), buffer, buffer + std::strlen(buffer));
	CRLF_count =  (_last_chunk_size != -1) ? _last_chunk_size : 0;
	for (size_t i = 0; i < _chunks.size(); i++)
	{
		if (_chunks[i] == '\r' && i + 1 <= _chunks.size() && _chunks[i + 1] == '\n')
		{
			CRLF_count++;
			end = i;
			if (CRLF_count % 2 == 1)
			{
				std::string number_str(it + start, it + end);
				aux.first = std::strtol(number_str.c_str(), NULL, 16);
				start = i + 2;
			}
			if (CRLF_count % 2 == 0)
			{
				std::string text_str(it + start, it + end);
				if (aux.first != static_cast<long>(text_str.size()))
				{
					_status = INVALID_REQUEST;
					return (set_validity(BAD_REQUEST, "Chunk lentgh doesn't match"));
				}
				_body.insert(_body.end(), it + start, it + end);
				start = i + 2;
			}
			if (aux.first == 0)
			{
				_status = FULL_COMPLETE_REQUEST;
				return _status;
			}
		}
	}
	return INVALID_REQUEST;
}

int	Request::manage_full_complete_request(char *buffer, int read_size)
{
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
		if (_req_accumulator[i] == '\r' && _req_accumulator[i + 1] == '\n'
			 && _req_accumulator[i + 2] == '\r' && _req_accumulator[i + 3] == '\n')
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
					break;
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
		spaces_trim(value);
		_headers[key] = value;
		if (key == "Host")
			set_host_and_port(value);
		if (key == "Content-Type")
			set_multipart_header(value);
	}
	return _valid;
}

void Request::set_multipart_header(std::string &value)
{
	std::string aux;
	std::string boundary_aux;
	size_t semicolon_position = value.find(";");
	if (semicolon_position != value.npos)
	{
		aux = value.substr(semicolon_position + 1);
		aux = spaces_trim(aux);
		size_t equal_position = aux.find("=");
		if (equal_position != aux.npos)
		{
			boundary_aux = aux.substr(0, equal_position);
			if (boundary_aux == "boundary")
			{
				_boundary = aux.substr(equal_position + 1);
			}
		}
		aux = value.substr(0, semicolon_position);
		if (aux == "multipart/form-data")
		{
			_multipart = true;
			_headers["Content-Type"] = aux;
		}
	}
	else if (value == "multipart/form-data")
	{
		_multipart = true;
	}
}

std::string Request::spaces_trim(std::string &str)
{
	std::string::iterator it = str.begin();
	while (it != str.end() && std::isspace(*it))
		++it;
	str.erase(str.begin(), it);
	return str;
}

bool Request::check_number_elements_request_line(std::vector<std::string> result)
{
	if (result.size() != 3)
		return set_validity(BAD_REQUEST);
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
	size_t i = 0;
	for (i = 0; i < _accept_method.size(); i++)
	{
		if (_accept_method[i] == _method)
			break;
	}
	if (i == _accept_method.size())
		return set_validity(METHOD_NOT_ALLOWED);
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
	return (this->_chunks);
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

bool	Request::getMultipart() const
{
	return (this->_multipart);
}

std::map<std::string, std::string> Request::getUploadFiles()
{
	return (this->upload_files);
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

void	Request::multipart()
{
	if (_multipart == 0)
		return ;
	else if (_multipart == 1 && _status == FULL_COMPLETE_REQUEST)
	{
		size_t pos;
		std::string copy_body;
		std::string body(_body.begin(), _body.end());
		_boundary.insert(0, "--");
		while (body.size() > _boundary.size() + 10)
		{
			if (body.find(_boundary) != std::string::npos)
				body.erase(body.find(_boundary), _boundary.size());
			if ((pos = body.find("\r\n\r\n")) != std::string::npos)
			{
				std::string headers = body.substr(0, pos);
				body.erase(0, pos + 4);
				copy_body = body.substr(0, (body.find(_boundary) - 2));
				body.erase(0, body.find(_boundary));
				if ((pos = headers.find("filename=\"")) != std::string::npos)
				{
					headers.erase(0, headers.find("filename=\"") + 10);
					pos = headers.find("\"");
					upload_files[headers.substr(0, pos)] = copy_body;
				}
			}
		}
	}
}

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

int    Request::check_request_line(std::vector<Server> &server)
{
	std::vector<Server>::iterator it_serv;

	for (it_serv = server.begin(); it_serv != server.end(); ++it_serv)
    {
		if (compareListen(it_serv->getListen()))
		{
			if (this->_host == it_serv->getServerName())
			    break;
			else
			{
				this->_error_code = BAD_REQUEST;
        		return (0);
			}
		}
    }
	if (it_serv != server.end())
	{
		if (compareMethod(*it_serv))
		{
			if (!it_serv->getLocation().empty())
				conf_loc = compareUri(it_serv->getLocation());
			if (conf_loc.getAutoindex() == -1)
			{
				if (_body_size > it_serv->getBodySize())
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
				if (_body_size > conf_loc.getBodySize())
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

void	Request::reset(void)
{
	_req_accumulator.clear();
	_method.clear();
	_uri.clear();
	_protocol.clear();
	_host.clear();
	_port = 0;
	_body.clear();
	_help_message.clear();
	_valid = true;
	_error_code = 200;
	_headers.clear();
	_params.clear();
	_body_size = 0;
	_chunks.clear();
	_accept_method.clear();
	_request_line.clear();
	_lines.clear();
	_type = 0;
	_status = EMPTY_REQUEST;
	_CRLFx2_index = 0;
	server_body_size = 0;
}
