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
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(addr_vect.sin_addr), ip, INET_ADDRSTRLEN);
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

int SocketManager::its_open(struct sockaddr_in &addr_vect)
{
    for (size_t a = 0; a < open_addr.size(); ++a)
    {
        if (open_addr[a].sin_port == addr_vect.sin_port && open_addr[a].sin_addr.s_addr == addr_vect.sin_addr.s_addr)
            return (0);
    }
    return (1);
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
            if (its_open(addr_vect[a]))
            {
                if (!connect_socket(pfds, addr_vect[a]))//si ha fallado algo al conectar el socket
                    std::cerr << "Error: Can't connect socket " << server[i].getServerName() << " to port." << std::endl;
                if (sock_num == BACKLOG - 3)//si hay demasiados puertos conectados
                {
                    std::cerr << "Can't connect all ports" << std::endl;
                    flag = 1;
                    break;
                }
                open_addr.push_back(addr_vect[a]);
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
        if (pfds[i].revents & POLLIN)//si hay algun evento
        {
            if (sock_num == BACKLOG - 1)
            {
                std::cerr << "Error: No hay espacio." << std::endl;//que hago si no hay mas espacio para almacenar las struct pollfd
                break;
            }
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            new_sock = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &client_len);
            if (new_sock != -1)//aceptar evento
            {
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

int SocketManager::is_file(int sock)
{
    std::map<int, int>::iterator it;

    for (it = fd_file.begin(); it != fd_file.end(); ++it)
    {
        if (it->second == sock)
            return (it->first);
    }
    return (0);
}

void    SocketManager::make_response(int sock, struct pollfd* pfds)
{
    pfds[sock].events = POLLOUT;//cambiamos en event de socket al POLLOUT, porque la request ya ha llegado entera y tenemos que responder al socke
    if (requests[sock].getLoc().getCGI() != -1)
        response[sock] = Response(requests[sock].getLoc(), requests[sock]);//crear la response de error
    else
        response[sock] = Response(requests[sock].getServ(), requests[sock]);
    pfds[sock_num].fd = response[sock].open_file(sock_num);
    //comprobar que hay flaf de cgi, si hay, no hacer nada
    if (pfds[sock_num].fd == -1)
    {
        fd_file[sock] = -1;
        response[sock].setStringBuffer(ErrorResponse());
        response[sock].setBytesRead(response[sock].getStringBuffer().size());
    }
    else
    {
        pfds[sock_num].events = POLLIN;
        fd_file[sock] = sock_num;
        sock_num++;
    }
}

void    SocketManager::check_join(int sock, struct pollfd* pfds, std::vector<Server> &server, char *buffer, int valread)
{
    requests[sock].set_current_status(INCOMPLETE_REQUEST);
    requests[sock].join_request(buffer, valread, server);
    if (requests[sock].get_error_code() != 200 || requests[sock].get_current_status() == FULL_COMPLETE_REQUEST)//juntar los request y ver si body es mas largo de lo permitido. Si esta mal hay que indicar el _error_code para generar la respuesta de error
        make_response(sock, pfds);
}

void    SocketManager::check_revent(struct pollfd* pfds, int client)
{
    printf("check_revent %d\n", client);//cambiar esta funcion
    if (pfds[client].revents & POLLERR) {
        printf("POLLERR %d\n", client);
        //close_move_pfd(pfds, client);
    }
    if (pfds[client].revents & POLLHUP) {
        printf("POLLHUP %d\n", client);
        //close_move_pfd(pfds, client);
    }
    if (pfds[client].revents & POLLNVAL) {
        printf("POLLNVAL %d\n", client);
        //close_move_pfd(pfds, client);
    }

}
void    SocketManager::recvRequest(struct pollfd* pfds, std::vector<Server> &server, int sock)
{
    char    buffer[BUFFER_SIZE + 1] = {0};
    int     valread;

    std::cout << "\033[32m" << " recvRequest " <<  "\033[0m" << std::endl;
    check_revent(pfds, sock);/// no se que hacer con esto!!!!!!!!!!!!!!!!!!!!
    if (sock_num == BACKLOG - 2)//si no hay espacio en pollfd para el fd del archivo
    {
        requests[sock].set_error_code(SERVICE_UNAVAIBLE);//en este caso mejor no abrir un archivo
        make_response(sock, pfds);
    }
    else
    {
        valread = recv(pfds[sock].fd, buffer, BUFFER_SIZE, 0);//recibimos el mensaje de socke
        if (requests[sock].get_current_status() == FULL_COMPLETE_REQUEST && valread == 0)
            make_response(sock, pfds);//ha terminado de recibir el mensaje
        else if (valread == 0 || valread == -1)
            close_move_pfd(pfds, sock);
        else
            check_join(sock, pfds, server, buffer, valread);//recibe una parte del mensaje
    }
    memset(buffer, 0, strlen(buffer));
}

void    SocketManager::readFile(struct pollfd* pfds, int sock, int file)
{
    int     valread;
    char    buffer[BUFFER_SIZE + 1] = {0};
    
    valread = read(pfds[sock].fd, buffer, BUFFER_SIZE);
    if (valread == -1)
        close_move_pfd(pfds, sock);
    if (response[file].getBytesRead() == 0)
        response[file].setStringBuffer(make_response_str(response[file], buffer));
    else
        response[file].setStringBuffer((buffer));//convirte char * en std::string
    response[file].setBytesRead(valread);
    memset(buffer, 0, strlen(buffer));
}

void    SocketManager::reventPOLLIN(struct pollfd* pfds, std::vector<Server> &server)
{
    int file;

    for (int sock = listen_sockets; sock < sock_num; sock++)//recorre todos los sockets
    {  
        if ((pfds[sock].revents & POLLIN))//si algun socket tiene un revent de POLLIN
        {
            if ((file = is_file(sock)))
                readFile(pfds, sock, file);
            else
                recvRequest(pfds, server, sock);
        }
        if (!is_file(sock) && difftime(time(NULL), requests[sock].get_time()) > 65)//si no hay evento y el tiempo es mayoa a 65, desconectamos al socke
        {
            //hay que mandar señal al hijo para que se cierre
            if (requests[sock].get_current_status() == EMPTY_REQUEST)//si no se ha reecibido ninguna request solo cierra la conexion
                close_move_pfd(pfds, sock);
            else if (response.find(sock) == response.end())
            {
                requests[sock].set_error_code(REQUEST_TIMEOUT);//si empezo a resibir request pero tarda mucho
                make_response(sock, pfds);
            }
        }
    }
}

void    SocketManager::sendResponse(struct pollfd* pfds)
{
    std::cout << "\033[32m" << " sendResponse " <<  "\033[0m" << std::endl;
    ssize_t send_size;

    for (int client = listen_sockets; client < sock_num; client++)//recorre todos los sockets
    {
        if ((pfds[client].revents & POLLOUT) && !is_file(client))//si algun socket tiene un revent de POLLOUT
        {
            send_size = send(pfds[client].fd, response[client].getStringBuffer().c_str(), response[client].getStringBuffer().size(), 0);//enviar el buffer leido de archivo
            response[client].setSendSize(send_size);
            if (static_cast<int>(response[client].getBytesRead()) == response[client].get_fileStat().st_size || response[client].getStringBuffer().empty())//significa que hemos llegado hasta el final del archivo
            {
                if (response[client].getConnectionVal() == CLOSE)
                    close_move_pfd(pfds, client);//cerrar conexion con el cliente
                else
                {
                    pfds[client].events = POLLIN;//volver a escuchar con el socket (ver cuando se sierra la conexion con el cliente)
                    response.erase(client);
                    requests[client].reset();
                    requests[client].last_conection_time();//guardar el tiempo de ultima conexion
                }
                close_move_pfd(pfds, fd_file[client]);//cerrar el fd de archivo
            }
            response[client].remove_sent_data(send_size);
        }
    }
}

void    SocketManager::close_move_pfd(struct pollfd* pfds, int pfd_free)
{
    std::cout << pfd_free << " CLOSEEEEEEE\n";
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
        requests.erase(sock_num - 1);//borramos ultima pos
        requests.erase(pfd_free);//borramos pos a eliminar
        requests[pfd_free] = copy_request;//y sustituimos
    }
    if (response.find(sock_num - 1) != response.end())
    {
        Response copy_response = response.find(sock_num - 1)->second;
        response.erase(sock_num - 1);
        response.erase(pfd_free);
        response[pfd_free] = copy_response;
    }
    if (fd_file.find(sock_num - 1) != fd_file.end())
    {
        int copy_fd = fd_file.find(sock_num - 1)->second;;
        fd_file.erase(sock_num - 1);
        fd_file.erase(pfd_free);
        fd_file[pfd_free] = copy_fd;
    }
    sock_num--;
}

std::string    SocketManager::ErrorResponse()
{
    return ("HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 25\r\n"
            "\r\n"
            "500 Internal Server Error");
}
std::string SocketManager::make_response_str(Response &response, std::string buffer)
{
    std::cout << "\033[32m" << " make_response_str " <<  "\033[0m" << std::endl;
    time_t rawtime;
    std::string day;
    struct tm *timeinfo;
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    const char* days[] = {"Mon", "Tues", "Weds", "Thur", "Fri", "Sat", "Sun"};

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::ostringstream str;

    str << status_code[response.getErrorCode()]
        << "Connection: " << response.getConnectionVal() << "\r\n"
        << "Date: " << days[timeinfo->tm_wday] << ", "
        << timeinfo->tm_mday << " "
        << months[timeinfo->tm_mon] << " "
        << (timeinfo->tm_year + 1900) << " "
        << timeinfo->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_min << ":"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_sec
        << " GMT\r\n"
        << "Content-Length: " << response.get_fileStat().st_size << "\r\n"
        << "\r\n" << buffer;
    return (str.str());
}
