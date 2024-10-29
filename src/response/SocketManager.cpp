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
                    //std::cout << "\033[33m" << " ********abro un socket ************* " <<  "\033[0m" << std::endl;
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(addr_vect.sin_addr), ip, INET_ADDRSTRLEN);
                    //std::cout << "  IP: " << ip << ", Port: " << ntohs(addr_vect.sin_port) << std::endl;
                    //std::cout << "Numero de socket: " << sock_num << std::endl;
                    pfds[sock_num].fd = sockfd;
                    pfds[sock_num].events = POLLIN;
                    sock_num++;
                    //std::cout << "\033[34m" << "connect_socket -> sock_num " << sock_num <<  "\033[0m" << std::endl; 
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
        //std::cout << pfds[i].fd << " Antes de revent " << i << std::endl;
        if (pfds[i].revents & POLLIN)//si hay algun evento
        {
            //std::cout << "Hay un revent " << i << std::endl;
            if (sock_num == BACKLOG - 1)
            {
                std::cerr << "Error: No hay espacio." << std::endl;//que hago si no hay mas espacio para almacenar las struct pollfd
                break;
            }
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            if ((new_sock = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &client_len)) != -1)//aceptar evento
            {
                std::cout << "\033[39m" << " Acepto a un cliente nuevo " << sock_num << ", del socket numero " << i <<  " \033[0m" << std::endl;
                pfds[sock_num].fd = new_sock;
                pfds[sock_num].events = POLLIN;
                requests[sock_num] = Request(sock_num, new_sock);
                requests[sock_num].last_conection_time();
                sock_num++;
                //std::cout << "\033[34m" << "acceptClient -> sock_num " << sock_num <<  "\033[0m" << std::endl;
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
        {
            //std::cout << client << " SOY ARCHIVO\n";
            return (1);
        }
    }
    //std::cout << client << " SOY SOCKET\n";
    return (0);
}

void    SocketManager::make_response(int client, struct pollfd* pfds)
{
    std::cout << "\033[35m" << " make response " <<  "\033[0m" << std::endl;
    pfds[client].events = POLLOUT;//cambiamos en event de socket al POLLOUT, porque la request ya ha llegado entera y tenemos que responder al cliente
    if (requests[client].getLoc().getCGI() != -1)
    {
        std::cout << "\033[35m" << " make response location" <<  "\033[0m" << std::endl;
        std::cout << requests[client].getLoc().getBodySize() << std::endl;
        response[client] = Response(requests[client].getLoc(), requests[client]);//crear la response de error
    }
    else
    {
        std::cout << "\033[35m" << " make response server" <<  "\033[0m" << std::endl;
        std::cout << requests[client].getServ().getBodySize() << std::endl;
        response[client] = Response(requests[client].getServ(), requests[client]);
    }
    pfds[sock_num].fd = response[client].open_file(sock_num);
    if (pfds[sock_num].fd == -1)
    {
        sendErrorResponse(pfds, client);
        close_move_pfd(pfds, client);
        close_move_pfd(pfds, sock_num);
    }
    pfds[sock_num].events = POLLIN;
    //std::cout << "\033[35m" << sock_num << " pfds[sock_num].fd es pipe " << pfds[sock_num].fd << "\033[0m" << std::endl;
    fd_file[client] = sock_num;
    sock_num++;
    //std::cout << "\033[34m" << "make_response -> sock_num " << sock_num <<  "\033[0m" << std::endl;
}

void    SocketManager::check_join(int client, struct pollfd* pfds, std::vector<Server> &server, char *buffer, int valread)
{
    std::cout << "\033[34m" << " RECIV REQUEST ... " << "\033[0m" << std::endl;
    requests[client].set_current_status(INCOMPLETE_REQUEST);
    requests[client].join_request(buffer, valread, server);
    if (requests[client].get_error_code() != 200)//juntar los request y ver si body es mas largo de lo permitido. Si esta mal hay que indicar el _error_code para generar la respuesta de error
    {
        std::cout << "\033[33m" << " STATUS CODE " << requests[client].get_error_code() << " \033[0m" << std::endl;
        pfds[client].events = POLLOUT;
        if (requests[client].getLoc().getAutoindex() != -1)
            response[client] = Response(requests[client].getLoc(), requests[client]);
        else
            response[client] = Response(requests[client].getServ(), requests[client]);//crear la response de error
        pfds[sock_num].fd = response[client].open_file(sock_num);//abre el archivo a enviar y retorna el numero fd (comprobar si se ha abierto bien el archivo, si hay error, enviar respuesta de error)
        if (pfds[sock_num].fd == -1)
        {
            sendErrorResponse(pfds, client);
            close_move_pfd(pfds, client);
            close_move_pfd(pfds, sock_num);
        }
        pfds[sock_num].events = POLLIN;//el evento para poder leer desde el archivo
        fd_file[client] = sock_num;//el fd a responder esta en posicion de sock_num
        sock_num++;
        //std::cout << "\033[34m" << "chek_join -> sock_num " << sock_num <<  "\033[0m" << std::endl;
    }
}



void    SocketManager::recvRequest(struct pollfd* pfds, std::vector<Server> &server)
{
    int     valread;
    char    buffer[BUFFER_SIZE + 1] = {0};

    std::cout << "SIZE REQUEST: " << requests.size() << " SIZE RESPONSE: " << response.size() << " SIZE FD " << fd_file.size() << std::endl;
    for (int i = 0; i < sock_num; ++i)
    {
        std::cout << "  Posicion en pollfd: " << i
                  << "  Descriptor fd: " << pfds[i].fd
                  << ", Eventos solicitados: " << pfds[i].events
                  << ", Eventos retornados: " << pfds[i].revents
                  << std::endl;
    }
    for (int client = listen_sockets; client < sock_num; client++)//recorre todos los sockets
    {
        //std::cout << "Soy sock_num " << sock_num << std::endl;
        //std::cout << "roooot " << response[client].getRoot() << std::endl;
        //std::cout << "\033[33m" << " Cliente " << client << " al que vamos a revisar " << "\033[0m" << std::endl;
        if (!is_file(client))
        {
            if ((pfds[client].revents & POLLIN))//si algun socket tiene un revent de POLLIN
            {
                std::cout << "\033[33m" << " Cliente " << client << " POLLIN!!!!!!!!!!!!!!!!!!!!!!!! " << "\033[0m" << std::endl;
                if (sock_num == BACKLOG - 1)//si no hay espacio en pollfd para el fd del archivo
                {
                    pfds[client].events = POLLOUT;
                    requests[client].set_error_code(SERVICE_UNAVAIBLE);
                    response[client] = Response(Server(), requests[client]);
                    fd_file[client] = -1;
                }
                else
                {
                    valread = recv(pfds[client].fd, buffer, BUFFER_SIZE, 0);//recibimos el mensaje de cliente
                    std::cout << "\033[36m" << " READ SIZE:  " << valread << " BUFFER:\n " << buffer << "\033[0m" << std::endl;
                    if (requests[client].get_current_status() == FULL_COMPLETE_REQUEST && valread == 0)
                        make_response(client, pfds);//ha terminado de recibir el mensaje
                    else if (valread == 0)
                        close_move_pfd(pfds, client);
                    else
                    {
                        check_join(client, pfds, server, buffer, valread);//recibe una parte del mensaje*
                        if (requests[client].get_current_status() == FULL_COMPLETE_REQUEST)
                            make_response(client, pfds);//ha terminado de recibir el mensaje
                    }
                    memset(buffer, 0, strlen(buffer));
                }
            }
            if (difftime(time(NULL), requests[client].get_time()) > 65)//si no hay evento y el tiempo es mayoa a 65, desconectamos al cliente
            {
                //std::cout << "\033[32m" << " TIMEOUT " << "\033[0m" << client << std::endl;
                if (requests[client].get_current_status() == EMPTY_REQUEST)//si no se ha reecibido ninguna request solo cierra la conexion
                    close_move_pfd(pfds, client);
                else if (response.find(client) == response.end())
                {
                    requests[client].set_error_code(REQUEST_TIMEOUT);//si empezo a resibir request pero tarda mucho
                    pfds[client].events = POLLOUT;
                    if (requests[client].getLoc().getAutoindex() != -1)
                        response[client] = Response(requests[client].getLoc(), requests[client]);
                    else
                        response[client] = Response(requests[client].getServ(), requests[client]);//crear la response de error
                    pfds[sock_num].fd = response[client].open_file(sock_num);//abre el archivo a enviar y retorna el numero fd (comprobar si se ha abierto bien el archivo, si hay error, enviar respuesta de error)
                    if (pfds[sock_num].fd == -1)
                    {
                        sendErrorResponse(pfds, client);
                        close_move_pfd(pfds, client);
                        close_move_pfd(pfds, sock_num);
                    }
                    pfds[sock_num].events = POLLIN;//el evento para poder leer desde el archivo
                    fd_file[client] = sock_num;//el fd a responder esta en posicion de sock_num
                    sock_num++;
                }
            }
            if (pfds[client].revents & POLLHUP)
            {
                printf("Cierre inesperado del cliente %d.\n", client);
                close_move_pfd(pfds, client);
            }
            if (pfds[client].revents & POLLERR)
            {
                printf("Error en el socket %d.\n", client);
                close_move_pfd(pfds, client);
            }
        }
    }
}

void    SocketManager::close_move_pfd(struct pollfd* pfds, int pfd_free)
{
    if (pfds[pfd_free].fd == -1)
        return ;
    close(pfds[pfd_free].fd);
    if (pfd_free == (sock_num - 1))//si el pfd que hay que eliminar esta en la ultima pos, lo borramos y ya
    {
        pfds[sock_num - 1].fd = -1;
        pfds[sock_num - 1].events = 0;
        pfds[sock_num - 1].revents = 0;
        requests.erase(sock_num - 1);
        response.erase(sock_num - 1);
        fd_file.erase(sock_num - 1);
        for (std::map<int, int>::iterator it = fd_file.begin(); it != fd_file.end(); ++it)
        {
            if (it->second == sock_num - 1)
            {
                fd_file.erase(it);
                break;
            }
        }
        sock_num--;
        return ;
    }
    //movemos el ultimo pfd a la pos borrada
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
        if (it->second == sock_num - 1)//si utima pos (la que vamos a mover) es un fd del archivo
        {
            it->second = pfd_free;//cambiamos pos de este archivo
            break;
        }
    }
    //cuando la structura movida ha sido un socket
    if (requests.find(sock_num - 1) != requests.end())//si este cliente tenia request
    {
        Request copy_request = requests.find(sock_num - 1)->second;//copiamos su instancia de request
        std::cout << copy_request.get_uri() << std::endl;
        requests.erase(sock_num - 1);//borramos ultima pos
        requests.erase(pfd_free);//borramos pos a eliminar
        requests[pfd_free] = copy_request;//y sustituimos
    }
    if (response.find(sock_num - 1) != response.end())
    {
        //std::cout << "Aqui 3\n";
        Response copy_response = response.find(sock_num - 1)->second;
        response.erase(sock_num - 1);
        response.erase(pfd_free);
        response[pfd_free] = copy_response;
    }
    if (fd_file.find(sock_num - 1) != fd_file.end())
    {
        //std::cout << "Aqui 4\n";
        int copy_fd = fd_file.find(sock_num - 1)->second;;
        fd_file.erase(sock_num - 1);
        fd_file.erase(pfd_free);
        fd_file[pfd_free] = copy_fd;
    }
    sock_num--;
}

void    SocketManager::sendErrorResponse(struct pollfd* pfds, int client)
{
    std::string response_buff;

    response_buff = "HTTP/1.1 500 Internal Server Error\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: 25\r\n"
                    "\r\n"
                    "500 Internal Server Error";
    send(pfds[client].fd, response_buff.c_str(), strlen(response_buff.c_str()), 0);//mandar el mensaje de error
}
std::string SocketManager::make_response_str(std::string string_buffer, int int_code, std::string connection_val, struct stat fileStat)
{
    time_t rawtime;
    std::string day;
    struct tm *timeinfo;
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    const char* days[] = {"Mon", "Tues", "Weds", "Thur", "Fri", "Sat", "Sun"};

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::ostringstream str;

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
        << "Content-Length: " << fileStat.st_size << "\r\n"
        << "\r\n" << string_buffer;
    std::cout << "cont len " << fileStat.st_size << std::endl;
    return (str.str());
}

void    SocketManager::sendResponse(struct pollfd* pfds)
{
    size_t valread;
    int _pos_file_response;
    std::string string_buffer;
    //std::string response_str;
    char    buffer[BUFFER_SIZE + 1] = {0};

    for (int i = listen_sockets; i < sock_num; i++)//recorre todos los sockets
    {
        //std::cout << "\033[33m" << " sendResponse " << sock_num << "\033[0m" << std::endl;
        if (!is_file(i))
        {
            if (pfds[i].revents & POLLOUT)//si algun socket tiene un revent de POLLOUT
            {
                std::cout << "\033[33m" << " Cliente " << i << " sin POLLOUT " << "\033[0m" << std::endl;
                _pos_file_response = fd_file[i];//posicion en pfds donde esta guardado el fd del archivo a enviar
                if (pfds[_pos_file_response].fd == -1)
                    {
                        std::cout << "\033[36m" << " ERRRRROOOOOOORRRRR " <<  "\033[0m" << std::endl;
                    //sendErrorResponse(pfds, i, _pos_file_response);
                    }
                else
                {
                    std::cout << "\033[33m" << " Cliente " << i << " POLLOUT " << "\033[0m" << std::endl;
                    if (pfds[_pos_file_response].revents & POLLIN)
                    {
                        valread = read(pfds[_pos_file_response].fd, buffer, BUFFER_SIZE);//leer del archivo a enviar BUFFER_SIZE bytes
                        string_buffer.assign(buffer);//convirte char * en std::string
                        if (response[i].getBytesRead() == 0)
                            string_buffer = make_response_str(string_buffer, response[i].getErrorCode(), response[i].getConnectionVal(), response[i].get_fileStat());
                        response[i].setBytesRead(valread);
                        send(pfds[i].fd, string_buffer.c_str(), string_buffer.size(), 0);//enviar el buffer leido de archivo
                        //std::cout << "\033[37m" << response[i].get_fileStat().st_size << " y " << valread << " SEND RESPONSE a " << i << "\n" << string_buffer << "\033[0m" << std::endl;
                        if (valread != BUFFER_SIZE || static_cast<int>(string_buffer.size()) == response[i].get_fileStat().st_size)//significa que hemos llegado hasta el final del archivo
                        {
                            if (response[i].getConnectionVal() == "close")
                            {
                            //std::cout << "\033[31m" << "YA HE MANDADO TODO" << "\033[0m" << std::endl;
                                close_move_pfd(pfds, i);//cerrar conexion con el cliente
                            }
                            else
                            {
                                //std::cout << "\033[33m" << " KEEP-ALIVE SOCKET " << "\033[0m" << std::endl;
                                pfds[i].events = POLLIN;//volver a escuchar con el socket (ver cuando se sierra la conexion con el cliente)
                                //resetear request
                                response.erase(i);
                                requests[i].reset();
                                requests[i].last_conection_time();//guardar el tiempo de ultima conexion

                            }
                            _pos_file_response = fd_file[i];
                            close_move_pfd(pfds, _pos_file_response);//cerrar el fd de archivo
                            //std::cout << "\033[31m" << "YA HE MANDADO TODO" << "\033[0m" << std::endl;
                            /*if (!response[i].getFileAutoindex().empty())
                            {
                                std::
                                remove(response[i].getFileAutoindex().c_str());
                            }*/
                            /*for (int i = 0; i < sock_num; ++i)
                            {
                                std::cout << "  Posicion en pollfd: " << i
                                          << "  Descriptor fd: " << pfds[i].fd
                                          << ", Eventos solicitados: " << pfds[i].events
                                          << ", Eventos retornados: " << pfds[i].revents
                                          << std::endl;
                            }*/
                        }
                        memset(buffer, 0, strlen(buffer));
                    }
                }
            }
        }
    }
}

/*
VER EL RFC!!!
*/