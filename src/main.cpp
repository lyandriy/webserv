# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <stdio.h>



int main()
{
	int socket_fd;
	struct sockaddr
	{
		unsigned short sa_family; // PF_*
		char sa_data[14]; // Dirección de protocolo.
	};

	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // protocolo 0 para q elija el sistema el más apropiado
	if (socket_fd < 0)
		perror("Wololo");
	else
		std::cout << socket_fd << std::endl;
	uint16_t port = htons(8080);
	std::cout << "Puerto: " << port << std::endl;
	port = ntohs(port);
	std::cout << "Puerto: " << port << std::endl;
	
	return 0;
}
