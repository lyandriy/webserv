# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <cerrno>

// lsof -i -P -n | grep webserv    <------    para comprobar puerto
// nc localhost 8080   <----    para hacer peticiones

int main()
{
	int socket_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in remote_addr;
	socklen_t addrlen;
	socklen_t len = sizeof(server_addr);
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
	getsockname(socket_fd, (struct sockaddr *)&server_addr, &len);
	std::cout << "Servidor escuchando en puerto: " << ntohs(server_addr.sin_port) << std::endl;
	int listener = listen(socket_fd, 8);
	if (listener < 0)
	{
		perror("Listener");
		std::cout << strerror(errno) << std::endl;
		exit(3);
	}
	addrlen = sizeof(struct sockaddr);
	int new_sock = accept(socket_fd, (struct sockaddr *)&remote_addr, &addrlen);
	if (new_sock < 0)
	{
		perror("New sock connect");
		std::cout << strerror(errno) << std::endl;
		exit(4);
	}

	char buffer[1024] = {0};
	int valread = read(new_sock, buffer, sizeof(buffer));
	if (valread <= 0)
	{
		perror("Error al leer del socket");
		std::cout << strerror(errno) << std::endl;
		exit(6);
	}
	else
	{
		const char *response = "He recibido la información y te envío otra de vuelta!!\n";
		std::cout << "Info recibida:\n" << buffer << std::endl;
		send(new_sock, response, strlen(response), 0);
	}
	
	close(new_sock);
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

