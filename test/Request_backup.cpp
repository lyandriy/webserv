Request::Request(int i, int fd, std::vector<char> request_accumulator, int type) : _fd_socket(fd), _pos_socket(i), 
						  _req_accumulator(request_accumulator), 
						  _method(""), _uri(""), _protocol(""), _host(""), _port(0),
						  _body(), _help_message(), _valid(true), _error_code(200), _headers(),
						  _params(), _chunks(), _accept_method(), _request_line(""), _lines()
{
	/*std::cout << "Constructor con índice, fd y acumulador llamado. Valor de i: " << i
				<<" valor de fd: " << fd << "\n";
	for (size_t i = 0; i < _req_accumulator.size(); i++)
		std::cout << _req_accumulator[i];
	std::cout << "\nFinal de la request\n" << std::endl;*/
	(void)type;
	
}

Request::Request(char *buffer) : _method(""), _uri(""), _protocol(""), _host(""), _port(0),
								_body(), _help_message(), _valid(true), _error_code(200),
								_headers(), _params(),
								_chunks(), _accept_method(), _request_line(""), _lines() 
{
	debug = true;
	if (debug == true)
	{
		std::cout << "\nConstructor BUFFER" << std::endl;
		_accept_method.push_back("GET");
		// _accept_metod.push_back("POST");
		_accept_method.push_back("DELETE");
	}
	this->_chunks.insert(_chunks.end(), buffer, buffer + std::strlen(buffer));
	// std::string aux(_chunks.begin(), _chunks.end());
	// _request_str = aux;
	read_request_lines();
	check_any_valid_line();
	extract_request_line();
	check_request_line();
	read_headers_lines();
	if (debug == true){print_request_complete_info();}
}
