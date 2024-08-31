# include "../inc/Common.hpp"
# include "../inc/Request.hpp"

// lsof -i -P -n | grep webserv    <------    para comprobar puerto
// nc localhost 8080   <----    para hacer peticiones

# define MAX_CLIENTS 20

int main()
{
	int socket_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in remote_addr;
	socklen_t addrlen;
	socklen_t len = sizeof(server_addr);
	struct pollfd poll_fd[MAX_CLIENTS + 1];
	int num_clientes = 0; (void)num_clientes;
	std::vector<Request> requests;
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
			if (poll_fd[i].revents & POLLIN)
			{
				int valread = recv(poll_fd[i].fd, buffer, sizeof(buffer), 0);
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
					const char *response = 
						"HTTP/1.1 200 OK\r\n"
						"Content-Type: text/html\r\n"
						"Connection: keep-alive\r\n"
						"Keep-Alive: timeout=5, max=1000\r\n"
						"Content-Length: 24\r\n"
						"\r\n"
						"<h1>Hola WOLOLO!!!</h1>\n";
					// std::cout << buffer << std::endl;
					Request prueba(buffer);
					requests.push_back(prueba);
					int diff = strncmp(buffer, "cerrar servidor", 15);
					if (diff == 0)
					{
						close(poll_fd[i].fd);
						close(socket_fd);
						return 0;
					}
					memset(buffer, 0, strlen(buffer));
					send(poll_fd[i].fd, response, strlen(response), 0);
				}
			}
		}
	}
	close(socket_fd);
	
	return 0;
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
