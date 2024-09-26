# include "../inc/Webserver.hpp"

SocketManager::SocketManager(){}

SocketManager::~SocketManager(){}

SocketManager::SocketManager(const SocketManager &other)
{
    *this = other;
}

SocketManager &SocketManager::operator=(const SocketManager &other)
{   
    this->response = other.response;
    this->requests = other.requests;
    this->listen_sockets = other.listen_sockets;
    this->sock_num = other.sock_num;
    return *this;
}

int SocketManager::getSockNum() const
{
    return (this->sock_num);
}

int SocketManager::connect_socket(struct pollfd* pfds, struct sockaddr_in &addr_vect)
{
    struct sockaddr_in  socket_addr;
    int optval = 1;
    int sockfd;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
    {
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != -1)
        {
            socket_addr = addr_vect;
            if (bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) != -1)
            {
                if (listen(sockfd, BACKLOG) != -1)
                {
                    pfds[sock_num].fd = sockfd;
                    pfds[sock_num].events = POLLIN;
                    sock_num++;
                    return (1);
                }
            }
        }
    }
    return (0);
}

SocketManager::SocketManager(struct pollfd* pfds, std::vector<Server> &server)
{
    this->sock_num = 0;
    this->listen_sockets = 0;
    int flag = 0;
    
    for (size_t i = 0; i < server.size(); ++i)//recorre los server
    {
        std::vector<struct sockaddr_in> addr_vect = server[i].getListen();
        for (size_t a = 0; a < addr_vect.size(); ++a)// recorre los sockaddr_in (los puetros que tiene un servidor)
        {
            if (!connect_socket(pfds, addr_vect[a]))//si ha fallado algo al conectar el socket
                std::cerr << "Error: Can't connect socket " << server[i].getServerName() << " to port." << std::endl;
            if (sock_num == BACKLOG - 1)//si hay demasiados puertos conectados
            {
                std::cerr << "Can't connect all ports" << std::endl;
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            break;
    }
    listen_sockets = sock_num;
    for (int i = sock_num; i < BACKLOG; i++)
        pfds[i].fd = -1;
}

int SocketManager::find_free_pos_socket(struct pollfd* pfds, int new_sock)
{
    for (int free_pfd = listen_sockets; free_pfd < BACKLOG; free_pfd++)//comprueba si hay espacio libre en pfds antes de sock_num
    {
        if (pfds[free_pfd].fd == -1)
        {
            pfds[free_pfd].fd = new_sock;
            pfds[free_pfd].events = POLLIN;
            requests[free_pfd] = Request(free_pfd, new_sock);
            return (1);
        }
    }
    close(new_sock);
    return (0);
}

void    SocketManager::acceptClient(struct pollfd* pfds, int ready)
{
    int new_sock;

    for (int i = 0; i < listen_sockets; i++)//recorrer los socket de escucha
    {
        if (pfds[i].revents & POLLIN)//si hay algun evento
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            if ((new_sock = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &client_len)) != -1)//aceptar evento
            {
                if (sock_num == BACKLOG)
                {
                    std::cerr << "Error: No hay espacio." << std::endl;//que hago si no hay mas espacio para almacenar las struct pollfd
                    close(new_sock);
                    break;
                }
                pfds[sock_num].fd = new_sock;
                pfds[sock_num].events = POLLIN;
                requests[sock_num] = Request(sock_num, new_sock);
                sock_num++;
            }
            else
                std::cerr << "Error: Accept client call." << std::endl;
        }
    }
}

void    SocketManager::make_response(int client, struct pollfd* pfds, std::vector<Server> &server)
{
    pfds[client].events = POLLOUT;//cambiamos en event de socket al POLLOUT, porque la request ya ha llegado entera y tenemos que responder al cliente
    if (response.find(client) != response.end())//si existe la instancia de response
    {
        requests[client].validity();//parsear la request
        response[client] = requests[client].request_resolution(server);//parsear con los datos de conf de server, si hay algun error, va a crear una response de error
        pfds[sock_num].fd = response[client].open_file(sock_num);
        fd_file[client] = sock_num;
        sock_num++;
    }
    else
    { 
        response[client] = Response(requests[client]);//crear la response de error
        pfds[sock_num].fd = response[client].open_file(sock_num);
        fd_file[client] = sock_num;
        sock_num++;
    }
}

void    SocketManager::check_join(int client, struct pollfd* pfds, std::vector<Server> &server, char *buffer)
{
    if (!requests[client].join_request(*buffer))//juntar los request y ver si body es mas largo de lo permitido. Si esta mal hay que indicar el _error_code para generar la respuesta de error
    {
        pfds[client].events = POLLOUT;
        response[client] = Response(requests[client]);//crear la response de error
        pfds[sock_num].fd = response[client].open_file(sock_num);//abre el archivo a enviar y retorna el numero fd (comprobar si se ha abierto bien el archivo, si hay error, enviar respuesta de error)
        pfds[sock_num].events = POLLIN;//el evento para poder leer desde el archivo
        fd_file[client] = sock_num;//el fd a responder esta en posicion de sock_num
        sock_num++;
    }
}

void    SocketManager::recvRequest(struct pollfd* pfds, std::vector<Server> &server)
{
    int     valread;
    char    buffer[BUFFER_SIZE + 1] = {0};

    for (int client = listen_sockets; client < sock_num; client++)//recorre todos los sockets
    {
        if (pfds[client].revents & POLLIN)//si algun socket tiene un revent de POLLIN
        {
            if (sock_num == BACKLOG)
            {
                std::cerr << "Error: No hay espacio." << std::endl;//que hago si no hay mas espacio para almacenar las struct pollfd
                break ;
            }
            valread = recv(pfds[client].fd, buffer, BUFFER_SIZE, 0);//recibimos el mensaje de cliente
            if (valread <= 0)
                make_response(client, pfds, server);//ha terminado de recibir el mensaje
            else
                check_join(client, pfds, server, buffer);//recibe una parte del mensaje
            memset(buffer, 0, strlen(buffer));
        }
    }
}

void    SocketManager::close_move_pfd(struct pollfd* pfds, int pfd_free)
{
    close(pfds[pfd_free].fd);
    if (pfd_free == sock_num)
    {
        pfds[sock_num].fd = -1;
        pfds[sock_num].events = 0;
        pfds[sock_num].revents = 0;
        return ;
    }
    ///cambiar ultimo pollfd a posicion de uno liberado
    pfds[pfd_free].fd = pfds[sock_num].fd;
    pfds[pfd_free].events = pfds[sock_num].events;
    pfds[pfd_free].revents = pfds[sock_num].revents;
    //vaciar el ultimo pollfd
    pfds[sock_num].fd = -1;
    pfds[sock_num].events = 0;
    pfds[sock_num].revents = 0;
    //cuando la structura movida era de fd del archivo
    for (std::map<int, int>::iterator it = fd_file.begin(); it != fd_file.end(); ++it)
    {
        if (it->second = sock_num)
            it->second = pfd_free;
    }
    //cuando la structura movida ha sido un socket
    std::map<int, Request>::iterator it = requests.find(sock_num);
    if (it != requests.end())
    {
        Request copy_request = it->second;
        requests.erase(sock_num);
        requests.erase(pfd_free);
        requests[pfd_free] = copy_request;
    }
    std::map<int, Response>::iterator it_ = response.find(sock_num);
    if (it_ != response.end())
    {
        Response copy_response = it_->second;
        response.erase(sock_num);
        response.erase(pfd_free);
        response[pfd_free] = copy_response;
    }
    std::map<int, int>::iterator _it = fd_file.find(sock_num);
    if (_it != fd_file.end())
    {
        int copy_fd = _it->second;;
        fd_file.erase(sock_num);
        fd_file.erase(pfd_free);
        fd_file[pfd_free] = copy_fd;
    }
    sock_num--;
}

void    SocketManager::sendResponse(struct pollfd* pfds)
{
    int     valread;
    int _pos_file_response;
    char    buffer[BUFFER_SIZE + 1] = {0};

    for (int i = listen_sockets; i <= sock_num; i++)//recorre todos los sockets
    {
        if (pfds[i].revents & POLLOUT)//si algun socket tiene un revent de POLLIN
        {
            _pos_file_response = fd_file[i];//posicion en pfds donde esta guardado el fd del archivo a enviar
            valread = read(pfds[_pos_file_response].fd, buffer, BUFFER_SIZE);//leer del archivo a enviar BUFFER_SIZE bytes
            if (valread == -1)//leemos de archivo BUFFER_SIZE bytes, si da error hay que responder con error code 503
            {
                const char *response_error = 
                    "HTTP/1.1 503 Service Unavailable\r\n"
                    "Content-Type: text/html\r\n"
                    "Retry-After: 3600\r\n"
                    "\r\n"
                    "<!DOCTYPE html>\r\n"
                    "<html lang=\"en\">\r\n"
                    "<head>\r\n"
                    "    <meta charset=\"UTF-8\">\r\n"
                    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n"
                    "    <title>503 Service Unavailable</title>\r\n"
                    "</head>\r\n"
                    "<body>\r\n"
                    "    <h1>503 Service Unavailable</h1>\r\n"
                    "    <p>The server is currently unable to handle your request due to maintenance or overloading. Please try again later.</p>\r\n"
                    "    <p>If the issue persists, contact support.</p>\r\n"
                    "</body>\r\n"
                    "</html>\r\n";
                send(pfds[i].fd, response_error, strlen(response_error), 0);//mandar el mensaje de error
                close_move_pfd(pfds, _pos_file_response);//cerrar el fd de archivo
                close_move_pfd(pfds, i);//cerrar el fd del cliente
            }
            else
            {
                send(pfds[i].fd, buffer, strlen(buffer), 0);//enviar el buffer leido de archivo
                if (valread != BUFFER_SIZE)//significa que hemos llegado hasta el final del archivo
                {
                    pfds[i].events = POLLIN;//volver a escuchar con el socket (ver cuando se sierra la conexion con el cliente)
                    close_move_pfd(pfds, _pos_file_response);//cerrar el fd de archivo
                    ///ver si el archivo que le mando es un error que cierra la conexion
                }
            }
        }
        memset(buffer, 0, strlen(buffer));
    }
}

/*
falta enviar HEADER
el socket del cliente se sierra cuando hay algun error o timeout. 
Si hay un error se manda el archivo de error, 
en el header se indica que se cierra la conexion con el socket.
VER EL RFC!!!
*/