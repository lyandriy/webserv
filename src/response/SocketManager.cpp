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
                    std::cout << "\033[33m" << "********abro un socket *************" <<  "\033[0m" << std::endl;
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(addr_vect.sin_addr), ip, INET_ADDRSTRLEN);
                    std::cout << "  IP: " << ip << ", Port: " << ntohs(addr_vect.sin_port) << std::endl;
                    std::cout << "Numero de socket: " << sock_num << std::endl;
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
    status_code[200] = "HTTP/1.1 200 OK\r\n";
    status_code[400] = "HTTP/1.1 400 Bad Request\r\n";
    status_code[403] = "HTTP/1.1 403 Forbidden\r\n";
    status_code[404] = "HTTP/1.1 404 Not Found\r\n";
    status_code[405] = "HTTP/1.1 405 Method Not Allowed\r\n";
    status_code[408] = "HTTP/1.1 408 Request timeout\r\n";
    status_code[413] = "HTTP/1.1 413 Content Too Large\r\n";
    status_code[414] = "HTTP/1.1 414 URI Too Long\r\n";
    status_code[500] = "HTTP/1.1 500 OInternal Server ErrorK\r\n";
    status_code[503] = "HTTP/1.1 503 Service Unavailable\r\n";
    status_code[505] = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
    
    for (size_t i = 0; i < server.size(); ++i)//recorre los server
    {
        std::vector<struct sockaddr_in> addr_vect = server[i].getListen();
        for (size_t a = 0; a < addr_vect.size(); ++a)// recorre los sockaddr_in (los puertos que tiene un servidor)
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

void    SocketManager::acceptClient(struct pollfd* pfds)
{
    int new_sock;

    for (int i = 0; i < listen_sockets; i++)//recorrer los socket de escucha
    {
        std::cout << "Antes de revent " << i << std::endl;
        if (pfds[i].revents & POLLIN)//si hay algun evento
        {
            std::cout << "Hay un revent " << i << std::endl;
            if (sock_num == BACKLOG - 1)
            {
                std::cerr << "Error: No hay espacio." << std::endl;//que hago si no hay mas espacio para almacenar las struct pollfd
                break;
            }
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            if ((new_sock = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &client_len)) != -1)//aceptar evento
            {
                std::cout << "\033[39m" << "Acepto a un cliente nuevo " << sock_num << ", del socket numero " << i <<  "\033[0m" << std::endl;
                pfds[sock_num].fd = new_sock;
                pfds[sock_num].events = POLLIN;
                requests[sock_num] = Request(sock_num, new_sock);
                requests[sock_num].last_conection_time();
                sock_num++;
            }
            else
                std::cerr << "Error: Accept client call." << std::endl;
        }
    }
}

int SocketManager::is_file(int client)
{
    std::map<int, int>::iterator it;

    for (it = fd_file.begin(); it != fd_file.end(); ++it)
    {
        if (it->second == client)
            return (1);
    }
    return (0);
}

void    SocketManager::make_response(int client, struct pollfd* pfds, std::vector<Server> &server)
{
    (void)server;
    pfds[client].events = POLLOUT;//cambiamos en event de socket al POLLOUT, porque la request ya ha llegado entera y tenemos que responder al cliente
    if (requests[client].get_current_status() != FULL_COMPLETE_REQUEST)//cuando ha ocurrido un error con recv y no ha habbido una lectura previa
    {
        requests[client].set_error_code(BAD_REQUEST);
        response[client] = Response(requests[client]);//crear la response de error
        pfds[sock_num].fd = response[client].open_file(sock_num);
        fd_file[client] = sock_num;
        sock_num++;
    }
    else
    {
        requests[client].set_error_code(OK);
        if (requests[client].getLoc().getBodySize() == -1)
            response[client] = Response(requests[client].getLoc(), requests[client]);//crear la response de error
        else
            response[client] = Response(requests[client].getServ(), requests[client]);
        pfds[sock_num].fd = response[client].open_file(sock_num);
        fd_file[client] = sock_num;
        sock_num++;
    }
}

void    SocketManager::check_join(int client, struct pollfd* pfds, std::vector<Server> &server, char *buffer, int valread)
{
    (void)server;
    (void)buffer;
    (void)valread;
    requests[client].set_error_code(200);
    requests[client].join_request(buffer, valread, server);
    if (requests[client].get_error_code() != 200)//juntar los request y ver si body es mas largo de lo permitido. Si esta mal hay que indicar el _error_code para generar la respuesta de error
    {
        pfds[client].events = POLLOUT;
        response[client] = Response(requests[client]);//crear la response de error
        pfds[sock_num].fd = response[client].open_file(sock_num);//abre el archivo a enviar y retorna el numero fd (comprobar si se ha abierto bien el archivo, si hay error, enviar respuesta de error)
        pfds[sock_num].events = POLLIN;//el evento para poder leer desde el archivo
        //std::cout << "\033[36m" << "sock_num " << sock_num <<  "\033[0m" << std::endl;
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
        if (!is_file(client) && (pfds[client].revents & POLLIN))//si algun socket tiene un revent de POLLIN
        {
            if (sock_num == BACKLOG - 1)//si no hay espacio en pollfd para el fd del archivo
            {
                pfds[client].events = POLLOUT;
                requests[client].set_error_code(SERVICE_UNAVAIBLE);
                response[client] = Response(requests[client]);
                fd_file[client] = -1;
            }
            else
            {
                valread = recv(pfds[client].fd, buffer, BUFFER_SIZE, 0);//recibimos el mensaje de cliente
                //std::cout << "\033[38m" << buffer <<  "\033[0m" << std::endl;
                if (valread <= 0)
                {
                    //std::cout << "\033[33m" << "estoy en make_response" <<  "\033[0m" << std::endl;
                    make_response(client, pfds, server);//ha terminado de recibir el mensaje
                }
                else
                {
                    std::cout << "\033[32m" << "estoy en check_join" <<  "\033[0m" << std::endl;
                    check_join(client, pfds, server, buffer, valread);//recibe una parte del mensaje
                }
                memset(buffer, 0, strlen(buffer));
            }
        }
        //std::cout << "\033[32m" << "Cliente " << client << " y su tiempo es " << difftime(time(NULL), requests[client].get_time()) << "\033[0m" << std::endl;
        if (!is_file(client) && difftime(time(NULL), requests[client].get_time()) > 10)//si no hay evento y el tiempo es mayoa a 65, desconectamos al cliente
        {
            if (requests[client].get_error_code() == 0)//si no se ha reecibido ninguna request solo cierra la conexion
                close_move_pfd(pfds, client);
            else if (response.find(client) == response.end())
            {
                requests[client].set_error_code(REQUEST_TIMEOUT);//si empezo a resibir request pero tarda mucho
                pfds[client].events = POLLOUT;
                response[client] = Response(requests[client]);//crear la response de error
                response[client].setConnectionVal("close"); 
                pfds[sock_num].fd = response[client].open_file(sock_num);//abre el archivo a enviar y retorna el numero fd (comprobar si se ha abierto bien el archivo, si hay error, enviar respuesta de error)
                std::cout << "\033[32m" << "posicion de fd de archivo " << sock_num << " y su numero es " << pfds[sock_num].fd <<  "\033[0m" << std::endl;
                std::cout << "\033[32m" << "posicion de cliente es  " << client << "\033[0m" << std::endl;
                pfds[sock_num].events = POLLIN;//el evento para poder leer desde el archivo
                fd_file[client] = sock_num;//el fd a responder esta en posicion de sock_num
                sock_num++;
            }
        }
    }
}

void    SocketManager::close_move_pfd(struct pollfd* pfds, int pfd_free)
{
    if (pfds[pfd_free].fd == -1)
        return ;
    std::cout << pfds[pfd_free].fd << "\n";
    close(pfds[pfd_free].fd);
    if (pfd_free == sock_num)
    {
        pfds[sock_num - 1].fd = -1;
        pfds[sock_num - 1].events = 0;
        pfds[sock_num - 1].revents = 0;
        return ;
    }
    std::cout << pfd_free << " " << sock_num - 1 << " aqui termino\n";
    //exit (0);
    ///cambiar ultimo pollfd a posicion de uno liberado
    pfds[pfd_free].fd = pfds[sock_num - 1].fd;
    pfds[pfd_free].events = pfds[sock_num - 1].events;
    pfds[pfd_free].revents = pfds[sock_num - 1].revents;
    //vaciar el ultimo pollfd
    pfds[sock_num - 1].fd = -1;
    pfds[sock_num - 1].events = 0;
    pfds[sock_num - 1].revents = 0;
    //cuando la structura movida era de fd del archivo
    for (std::map<int, int>::iterator it = fd_file.begin(); it != fd_file.end(); ++it)
    {
        if (it->second == sock_num - 1)
            it->second = pfd_free;
    }
    //cuando la structura movida ha sido un socket
    std::map<int, Request>::iterator it = requests.find(sock_num - 1);
    if (it != requests.end())
    {
        Request copy_request = it->second;
        requests.erase(sock_num - 1);
        requests.erase(pfd_free);
        requests[pfd_free] = copy_request;
    }
    std::map<int, Response>::iterator it_ = response.find(sock_num - 1);
    if (it_ != response.end())
    {
        Response copy_response = it_->second;
        response.erase(sock_num - 1);
        response.erase(pfd_free);
        response[pfd_free] = copy_response;
    }
    std::map<int, int>::iterator _it = fd_file.find(sock_num - 1);
    if (_it != fd_file.end())
    {
        int copy_fd = _it->second;;
        fd_file.erase(sock_num - 1);
        fd_file.erase(pfd_free);
        fd_file[pfd_free] = copy_fd;
    }
    sock_num--;
}

void    SocketManager::sendErrorResponse(struct pollfd* pfds, int i, int _pos_file_response)
{
    std::string response_buff;
    
    if (response[i].getErrorCode() == SERVICE_UNAVAIBLE)
        response_buff = "HTTP/1.1 503 Service Unavaible\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: 30\r\n"
                    "\r\n"
                    "<h1>503 Service Unavaible</h1>";
    else if (response[i].getErrorCode() == INTERNAL_SERVER_ERROR)
        response_buff = "HTTP/1.1 500 Internal Server Error\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: 35\r\n"
                        "\r\n"
                        "<h1>500 Internal Server Error</h1>";
    send(pfds[i].fd, response_buff.c_str(), strlen(response_buff.c_str()), 0);//mandar el mensaje de error
    pfds[i].events = POLLIN;
    requests[i].last_conection_time();
    if (fd_file[i] != -1)//si el archivo ha sido abierto
        close_move_pfd(pfds, _pos_file_response);//cerrar el fd de archivo
}
std::string SocketManager::make_response_str(std::string string_buffer, int int_code, std::string connection_val)
{
    time_t rawtime;
    std::string day;
    struct tm *timeinfo;
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    const char* days[] = {"Mon", "Tues", "Weds", "Thur", "Fri", "Sat", "Sun"};

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::ostringstream str;

    //std::cout << "\033[30m" << "status code " << status_code[int_code] <<  "\033[0m" << std::endl;
    //std::cout << "\033[30m" << "int_code " << int_code <<  "\033[0m" << std::endl;
    str << status_code[int_code]
        << "Connection: " << connection_val << "\r\n"
        << "Date: " << days[timeinfo->tm_wday] << ", "
        << timeinfo->tm_mday << " " 
        << months[timeinfo->tm_mon] << " "
        << (timeinfo->tm_year + 1900) << " " 
        << timeinfo->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_min << ":"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_sec
        << " GMT\r\n"
        << "Content-Length: " << string_buffer.size() << "\r\n"
        << "\r\n" << string_buffer;

    return (str.str());
}

void    SocketManager::sendResponse(struct pollfd* pfds)
{
    int valread;
    int _pos_file_response;
    std::string string_buffer;
    std::string response_str;
    char    buffer[BUFFER_SIZE + 1] = {0};

    for (int i = listen_sockets; i <= sock_num; i++)//recorre todos los sockets
    {
        if (pfds[i].revents & POLLOUT)//si algun socket tiene un revent de POLLOUT
        {
            _pos_file_response = fd_file[i];//posicion en pfds donde esta guardado el fd del archivo a enviar
            if (response[i].getErrorCode() == INTERNAL_SERVER_ERROR
                || response[i].getErrorCode() == SERVICE_UNAVAIBLE)
                sendErrorResponse(pfds, i, _pos_file_response);
            else
            {
                if (pfds[_pos_file_response].revents & POLLIN)
                {
                    std::cout << "fd archivo revent es POLLIN\n";
                    valread = read(pfds[_pos_file_response].fd, buffer, BUFFER_SIZE);//leer del archivo a enviar BUFFER_SIZE bytes
                    //std::cout  << "\033[33m" << valread << "<-valread" <<  buffer  << std::endl;
                    string_buffer.assign(buffer);//convirte char * en std::string
                    response_str = make_response_str(string_buffer, response[i].getErrorCode(), response[i].getConnectionVal());
                    //std::cout << "\033[33m" << "Soy cliente " << i <<  "\033[0m" << std::endl;
                    //std::cout << "\033[32m" << response_str <<  "\033[0m" << std::endl;
                    //requests[i].print_full_info();
                    send(pfds[i].fd, response_str.c_str(), response_str.size(), 0);//enviar el buffer leido de archivo
                    if (valread != BUFFER_SIZE || static_cast<int>(string_buffer.size()) == response[i].get_fileStat().st_size)//significa que hemos llegado hasta el final del archivo
                    {
                        std::cout << response[i].getConnectionVal()  << "voy a cerrar la conexion\n";
                        if (response[i].getConnectionVal() == "close")
                            close_move_pfd(pfds, i);//cerrar conexion con el cliente
                        else
                        {
                            pfds[i].events = POLLIN;//volver a escuchar con el socket (ver cuando se sierra la conexion con el cliente)
                            requests[i].last_conection_time();//guardar el tiempo de ultima conexion
                        }
                        close_move_pfd(pfds, _pos_file_response);//cerrar el fd de archivo
                    }
                }
            }
        }
    }
}

/*
VER EL RFC!!!
*/