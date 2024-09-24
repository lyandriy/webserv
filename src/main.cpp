# include "../inc/Common.hpp"
# include "../inc/Request.hpp"

// lsof -i -P -n | grep webserv    <------    para comprobar puerto
// nc localhost 8080   <----    para hacer peticiones

# define MAX_CLIENTS 20
# define BUFFER_SIZE 24

void print_raw_request(char *buffer);

int check_request_complete(std::vector<char> &request_accumulator)
{
	std::string request(request_accumulator.begin(), request_accumulator.end());
	size_t header_end_pos;

//buscar final de los encabezados o de la propia request
	header_end_pos = request.find("\r\n\r\n");
	if (header_end_pos == std::string::npos)
		return INCOMPLETE_REQUEST;
	
//buscar header de Content-Length
	std::string content_length_str = "Content-Length:";
	size_t content_length_pos = request.find(content_length_str);
	if (content_length_pos != std::string::npos)
	{
		return REQUEST_WITH_BODY;
		// size_t content_length_start = content_length_pos + content_length_str.size();
		// size_t content_length_end = request.find("\r\n", content_length_start);
		// content_length_str = request.substr(content_length_start, content_length_end - content_length_start);
		// int content_length = std::atoi(content_length_str.c_str());
		// (void) content_length;
	}

//buscar header Transfer-Enconding: chunked
	size_t transfer_encoding_pos = request.find("Transfer-Encoding: chunked");
	if (transfer_encoding_pos == std::string::npos)
		transfer_encoding_pos = request.find("Transfer-Encoding: chunked");
	if (transfer_encoding_pos != std::string::npos)
	{
		return CHUNKED_REQUEST;
		// size_t chunck_end = request.find("0\r\n\r\n", header_end_pos + 4);
		// if (chunck_end != std::string::npos)
		// 	return true;
	}

	return COMPLETE_REQUEST;
	// return true;
}

int main()
{
	int socket_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in remote_addr;
	socklen_t addrlen;
	socklen_t len = sizeof(server_addr);
	struct pollfd poll_fd[MAX_CLIENTS + 1];
	int num_clientes = 0; (void)num_clientes;
	std::vector<Request> requests(MAX_CLIENTS);
	std::vector<std::vector<char> > request_accumulators(MAX_CLIENTS);
	(void)poll_fd;
	(void)len;


	// socket_fd = socket(AF_INET, SOCK_STREAM, 0); // protocolo 0 para q elija el sistema el más apropiado
	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // protocolo 0 para q elija el sistema el más apropiado
	if (socket_fd < 0)
	{
		perror("Wololo");
		std::cout << strerror(errno) << std::endl;
		exit(1);
	}
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("Error en setsockectopt");
		std::cout << strerror(errno) << std::endl;
		exit(5);
	}

	server_addr.sin_family = PF_INET;
	// sin.sin_port = 8080;
	server_addr.sin_port = htons(8080);
	// sin.sin_addr.s_addr = inet_addr("127.0.1.1");
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	std::cout << "Socketfd: " << socket_fd << std::endl;
	int ret = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0)
	{
		perror("Error binding the socket");
		std::cout << strerror(errno) << std::endl;
		exit(2);
	}
	// getsockname(socket_fd, (struct sockaddr *)&server_addr, &len);
	std::cout << "Servidor escuchando en puerto: " << ntohs(server_addr.sin_port) << std::endl;
	int listener = listen(socket_fd, 8);
	if (listener < 0)
	{
		perror("Listener");
		std::cout << strerror(errno) << std::endl;
		exit(3);
	}
	int new_sock;
	char buffer[1024] = {0};

	poll_fd[0].fd = socket_fd;
	poll_fd[0].events = POLLIN;
	while (true)
	{
		int poll_count = poll(poll_fd, num_clientes + 1, -1);
		if (poll_count < 0)
		{
			perror("Poll error");
			std::cerr << strerror(errno) << std::endl;
			exit(7);
		}
		if (poll_fd[0].revents & POLLIN)
		{
			addrlen = sizeof(struct sockaddr);
			new_sock = accept(socket_fd, (struct sockaddr *)&remote_addr, &addrlen);
			if (new_sock < 0)
			{
				perror("New sock connect");
				std::cout << strerror(errno) << std::endl;
				exit(4);
			}
			if (num_clientes < MAX_CLIENTS)
			{
				poll_fd[num_clientes + 1].fd = new_sock;
				poll_fd[num_clientes + 1].events = POLLIN;
				// requests.push_back(Request(num_clientes, poll_fd[num_clientes].fd));
				num_clientes++;
				std::cout << "Nuevo cliente conectado, en total hay " << num_clientes << std::endl;
			}
			else
			{
				std::cerr << "Máximo de clientes alcanzados, se cierra la nueva conexión" << std::endl;
				close(new_sock);
			}
		}
		for (int i = 1; i <= num_clientes; i++)
		{
			// std::cout << "POLLFD[" << i << "] Revents: " << poll_fd[i].revents << std::endl; 
			if (poll_fd[i].revents & POLLIN)
			{
				int valread = recv(poll_fd[i].fd, buffer, BUFFER_SIZE, 0);
				// sleep(5);
				if (valread <= 0)
				{
					if (valread == 0)
					{
						std::cout << "El cliente cerró la conexión" << std::endl;
					}
					else
					{
						perror("Error al leer del socket");
						std::cout << strerror(errno) << std::endl;
					}
						close(poll_fd[i].fd);
						poll_fd[i] = poll_fd[num_clientes];
						num_clientes--;
						i--;
				}
				else
				{
					const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nKeep-Alive: timeout=2, max=10\r\nContent-Length: 22\r\n\r\n<h1>Hola ELOY!!!</h1>\n";
					print_raw_request(buffer);

					if (i <= MAX_CLIENTS)
					request_accumulators[i].insert(request_accumulators[i].end(), buffer, buffer + valread);
					int request_complete = check_request_complete(request_accumulators[i]);
					if (request_complete > 0)
						requests[i] = Request(i, poll_fd[i].fd, request_accumulators[i], request_complete);


					/* switch (check_request_complete(request_accumulators[i]))
					{
					case COMPLETE_REQUEST:
						requests[i] = Request(i, poll_fd[i].fd, request_accumulators[i], COMPLETE_REQUEST);
						break;
					case REQUEST_WITH_BODY:
						requests[i] = Request(i, poll_fd[i].fd, request_accumulators[i], REQUEST_WITH_BODY);
						break;
					case CHUNKED_REQUEST:
						requests[i] = Request(i, poll_fd[i].fd, request_accumulators[i], CHUNKED_REQUEST);
						break;
					default:
						std::cout << "\nRequest incompleta\n";
						break;
					} */
					/* if (check_request_complete(request_accumulators[i]) > 0)
					{
						//procesar la petición, crear la instancia de Request etc...
						std::cout << "\nRequest lista para ser procesada\n";
						Request prueba(i, poll_fd[i].fd, request_accumulators[i]);
						if (prueba.get_validity() == true)
							requests.push_back(prueba);
					}
					else
					{
						std::cout << "\nRequest incompleta\n";
						// for (size_t j = 0; j < request_accumulators[i].size(); j++)
						// {
						// 	std::cout << request_accumulators[i][j];
						// }
						// std::cout << std::endl;
					}
					
					if (strncmp(buffer, "cerrar servidor", 15) == 0)
					{
						close(poll_fd[i].fd);
						close(socket_fd);
						return 0;
					} */
					memset(buffer, 0, BUFFER_SIZE);
					send(poll_fd[i].fd, response, strlen(response), 0);
				}
			}
		}
	}
	close(socket_fd);
	
	return 0;
}

void print_raw_request(char * buffer)
{
	for (size_t i = 0; i < strlen(buffer); i++)
	{
		if (buffer[i] == '\r')
			std::cout << "\\r";
		else if (buffer[i] == '\n')
			std::cout << "\\n";
		else
			std::cout << buffer[i];
	}
}

/* 

		else
		{
			std::cout << "ret (bind): " << ret << std::endl;
			std::cout << "port: " << server_addr.sin_port << std::endl;
			if (listener < 0)
				perror("Listener");
			else
			{
				addrlen = sizeof(struct sockaddr);
				std::cout << "Listener: " << listener << std::endl;
				struct sockaddr_in remote_addr;
				int acpt = accept(socket_fd, (struct sockaddr *)&sin, sizeof(sin));
			}
		}
	uint16_t port = htons(8080);
	std::cout << "Puerto: " << port << std::endl;
	port = ntohs(port);
	std::cout << "Puerto: " << port << std::endl; */

/* struct sockaddr
{
	unsigned short sa_family;
	char sa_data[14];
};

struct sockaddr_in
{
	short int sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	unsigned char sin_zero[8];
};

struct in_addr
{
	unsigned long s_addr;
};
 */

		// int ret = poll(poll_fd, 2, 1000);
