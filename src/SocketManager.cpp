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

int SocketManager::connect_socket(struct sockaddr_in &addr_vect)
{
    struct sockaddr_in  socket_addr;
    int optval = 1;
    int sockfd;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
    {
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != -1)
        {
            if (fcntl(sockfd, F_SETFD, O_CLOEXEC) != -1 || fcntl(sockfd, F_SETFL, O_NONBLOCK) != -1)
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

SocketManager::SocketManager(struct pollfd* pfds_, std::vector<Server> &server)
{
    this->sock_num = 0;
    this->listen_sockets = 0;
    int flag = 0;
    pfds = pfds_;
    status_code[200] = "HTTP/1.1 200 OK\r\n";
    status_code[400] = "HTTP/1.1 400 Bad Request\r\n";
    status_code[403] = "HTTP/1.1 403 Forbidden\r\n";
    status_code[404] = "HTTP/1.1 404 Not Found\r\n";
    status_code[405] = "HTTP/1.1 405 Method Not Allowed\r\n";
    status_code[408] = "HTTP/1.1 408 Request timeout\r\n";
    status_code[413] = "HTTP/1.1 413 Content Too Large\r\n";
    status_code[414] = "HTTP/1.1 414 URI Too Long\r\n";
    status_code[500] = "HTTP/1.1 500 Internal Server Error\r\n";
    status_code[503] = "HTTP/1.1 503 Service Unavailable\r\n";
    status_code[504] = "HTTP/1.1 504 Gateway Timeout\r\n";
    status_code[505] = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
    
    for (size_t i = 0; i < server.size(); ++i)//recorre los server
    {
        std::vector<struct sockaddr_in> addr_vect = server[i].getListen();
        for (size_t a = 0; a < addr_vect.size(); ++a)// recorre los sockaddr_in (los puertos que tiene un servidor)
        {
            if (its_open(addr_vect[a]))
            {
                if (!connect_socket(addr_vect[a]))//si ha fallado algo al conectar el socket
                {
                    std::cerr << "Error: Can't connect socket " << server[i].getServerName() << " to port." << std::endl;
                    exit(1);
                }
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

int SocketManager::free_pos()
{
    for (int i = 0; i < BACKLOG - 1; i++)
    {
        if (pfds[i].fd == -1 && !is_file(i))
            return (i);   
    }
    return (0);
}

void    SocketManager::acceptClient()
{
    int new_sock;
    int pos_free;

    for (int i = 0; i < listen_sockets; i++)//recorrer los socket de escucha
    {
        if ((pfds[i].revents & POLLIN) && pfds[i].fd != -1)//si hay algun evento
        {
            pos_free = free_pos();
            if (pos_free == BACKLOG - 1)
            {
                std::cerr << "Error: No hay espacio." << std::endl;//que hago si no hay mas espacio para almacenar las struct pollfd
                break;
            }
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            new_sock = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &client_len);
            if (new_sock != -1 || fcntl(new_sock, F_SETFD, O_CLOEXEC) != -1 || fcntl(new_sock, F_SETFL, O_NONBLOCK) != -1)//aceptar evento
            {
                pfds[pos_free].fd = new_sock;
                pfds[pos_free].events = POLLIN;
                std::cout << "\033[33m" << pos_free << " acceptClient " << pfds[pos_free].fd << "\033[0m" << std::endl;
                requests[pos_free] = Request(pos_free, new_sock);
                requests[pos_free].last_conection_time();
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

void   SocketManager::ErrorResponse(Response &response, int &fd_file, int sock)
{
    fd_file = -1;
    response.setStringBuffer("HTTP/1.1 500 Internal Server Error\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Content-Length: 25\r\n"
                                    "\r\n"
                                    "500 Internal Server Error");
    response.setBytesRead(response.getStringBuffer().size());
    pfds[sock].events = POLLOUT;
}

int SocketManager::deleteMethod(int sock)
{
    int result;
    int fd = -1;
    /* struct stat info;

    try{
        if (stat("deleted", &info) == 0 && (info.st_mode & S_IFDIR))
        {
            std::cout << "EL DIRECTORIO DELETE EXISTE" << std::endl;
        }
        else
        {
            std::cout << "EL DIRECTORIO delete no existe" << std::endl;
        }
    }
    catch (...)
    {
    std::cout << "ha habido un error" <<  std::endl;} */

    std::string newname = "deleted" + response[sock].getURI();
    // std::cout << "--->>> " << newname << " <<<---" << std::endl;
    result = rename(response[sock].getRoot().c_str(), newname.c_str());
    // std::cout << "--->>> " << response[sock].getRoot() << " <<<---" << std::endl;
    if (result == 0)
        {
            fd = response[sock].get_fd("serverHTML/deletedResponse.html");
        }
    if (result != 0)
    {
        response[sock].setErrorCode(NOT_FOUND);
        fd = response[sock].open_error_file();
    }
    else if (fd == -1)
    {
        response[sock].setErrorCode(INTERNAL_SERVER_ERROR);
        fd = response[sock].open_error_file();
    }
    return(fd);
}

void    SocketManager:: make_response(int sock)
{
    int pos_free = free_pos();
    std::cout << "\033[32m" << "make_response" << "\033[0m" << std::endl;
    if (requests[sock].getLoc().getCGI() != -1)
        response[sock] = Response(requests[sock].getLoc(), requests[sock]);
    else
        response[sock] = Response(requests[sock].getServ(), requests[sock]);
    if (requests[sock].getMultipart() == true)
        pfds[pos_free].fd = response[sock].makePost();        
    else
        pfds[pos_free].fd = response[sock].open_file(pos_free);
    if (response[sock].getCGIState() == 1)
        cgiClients[sock] = CGI(response[sock]);
    else if (response[sock].getErrorCode() == 200 && requests[sock].get_method() == "POST" && !response[sock].getPipeRes())
    {
        if (response[sock].postIsExec())
            cgiClients[sock] = CGI(response[sock]);
        else
        {
            pfds[pos_free].fd = response[sock].makePost();
            pfds[pos_free].events = POLLIN;
            fd_file[sock] = pos_free;
            pfds[sock].events = POLLOUT;
        }
    }
    else if (pfds[pos_free].fd == -1)
        ErrorResponse(response[sock], fd_file[sock], sock);
    else if (response[sock].getErrorCode() == 200 && requests[sock].get_method() == "DELETE")
    {
        close(pfds[pos_free].fd);
        pfds[pos_free].fd = deleteMethod(sock);
        pfds[pos_free].events = POLLIN;
        pfds[sock].events = POLLOUT;
        fd_file[sock] = pos_free;
    }
    else
    {
        pfds[pos_free].events = POLLIN;
        fd_file[sock] = pos_free;
        pfds[sock].events = POLLOUT;
    }
}

void    SocketManager::check_join(int sock, std::vector<Server> &server, char *buffer, int valread)
{
    requests[sock].join_request(buffer, valread, server);
    if (requests[sock].get_error_code() != 200 || requests[sock].get_current_status() == FULL_COMPLETE_REQUEST)//juntar los request y ver si body es mas largo de lo permitido. Si esta mal hay que indicar el _error_code para generar la respuesta de error
        make_response(sock);
}

void    SocketManager::check_revent(int client)
{
    if (pfds[client].revents & POLLERR) {
        printf("POLLERR %d\n", client);
        close_pfd(client);
    }
    if (pfds[client].revents & POLLHUP) {
        printf("POLLHUP %d\n", client);
        close_pfd(client);
    }
    if (pfds[client].revents & POLLNVAL) {
        
        printf("POLLNVAL %d\n", client);
        close_pfd(client);
    }
    if (pfds[client].revents & POLLRDHUP){
        printf("POLLRDHUP %d\n", client);
    }
}
void    SocketManager::recvRequest(std::vector<Server> &server, int sock)
{
    char    buffer[BUFFER_SIZE + 1] = {0};
    int     valread;
    int pos_free = free_pos();

    std::cout << "\033[33m" << " recvRequest " << "\033[0m" << std::endl;
    check_revent(sock);/// no se que hacer con esto!!!!!!!!!!!!!!!!!!!!
    if (pos_free == BACKLOG - 2)//si no hay espacio en pollfd para el fd del archivo
    {
        requests[sock].set_error_code(SERVICE_UNAVAIBLE);//en este caso mejor no abrir un archivo
        ErrorResponse(response[sock], fd_file[sock], sock);
        //make_response(sock);
    }
    else
    {
        valread = recv(pfds[sock].fd, buffer, BUFFER_SIZE, 0);//recibimos el mensaje de socke //ver por que es 0
        std::cout << "\033[35m" << "Client " << sock << std::endl << buffer << "\033[0m" << std::endl;
        if (requests[sock].get_current_status() == FULL_COMPLETE_REQUEST && valread == 0 && response.find(sock) == response.end())
            make_response(sock);//ha terminado de recibir el mensaje
        else if (valread == 0 || valread == -1)
            close_pfd(sock);
        else
            check_join(sock, server, buffer, valread);//recibe una parte del mensaje
    }
    memset(buffer, 0, strlen(buffer));
}

void    SocketManager::readFile(int sock, int file)
{
    int     valread;
    char    buffer[BUFFER_SIZE + 1] = {0};

    valread = read(pfds[sock].fd, buffer, BUFFER_SIZE);
    //std::cout << "\033[32m" << buffer << "\n readFile " << file << "\033[0m" << std::endl;
    if (valread == -1)
    {
        response[is_file(sock)].setBytesRead(0);
        close_pfd(sock);
    }
    //si el fd es pipe o tamaño del achivo es mayr a BUFFER_SIZE hacer chunkd response en otros casos respuesta normal
    if (response[file].get_fileStat().st_size > BUFFER_SIZE || response[file].getPipeRes())
    {
        if (response[file].getBytesRead() == 0)
            response[file].setStringBuffer(make_chunked_response(response[file], buffer, valread));
        else
            response[file].setStringBuffer(make_chunked(buffer, valread));
        response[file].setBytesRead(valread);//que hago con esto?
        response[file].setValread(valread);
    }
    else//respuesta Content-Length
    {
        if (response[file].getBytesRead() == 0)
            response[file].setStringBuffer(make_response_str(response[file], buffer));
        else
            response[file].setStringBuffer((buffer));//convirte char * en std::string
        response[file].setBytesRead(valread);
    }
    memset(buffer, 0, strlen(buffer));
}

void    SocketManager::reventPOLLIN(std::vector<Server> &server)
{
    int file;

    std::cout << "\033[34m" << " reventPOLLIN " << "\033[0m" << std::endl;
    for (int sock = listen_sockets; sock < BACKLOG; sock++)//recorre todos los sockets
    {  
        if ((pfds[sock].revents & POLLIN) && pfds[sock].fd != -1)//si algun socket tiene un revent de POLLIN
        {
            check_revent(sock);
            if ((file = is_file(sock)))
                readFile(sock, file);
            else
                recvRequest(server, sock);
        }
        if (requests.find(sock) != requests.end() && !is_file(sock) && pfds[sock].fd != -1 && difftime(time(NULL), requests[sock].get_time()) > 65)//si no hay evento y el tiempo es mayoa a 65, desconectamos al socke
        {
            if (requests[sock].get_current_status() == EMPTY_REQUEST)//si no se ha reecibido ninguna request solo cierra la conexion
                    close_pfd(sock);
            else if (response.find(sock) == response.end())
            {
                requests[sock].set_error_code(REQUEST_TIMEOUT);//si empezo a resibir request pero tarda mucho
                make_response(sock);
            }
            else if (cgiClients.find(sock) != cgiClients.end() && cgiClients[sock].getPid() != -1)
            {
                kill(cgiClients[sock].getPid(), SIGINT);
                cgiClients.erase(sock);
                response.erase(sock);
                requests[sock].set_error_code(GATEWAY_TIMEOUT);//si empezo a resibir request pero tarda mucho
                make_response(sock);
            }
            requests[sock].last_conection_time();
        }        
    }
}

void    SocketManager::managerFinishSend(int client)
{
    if (response[client].getConnectionVal() == CLOSE)
        close_pfd(client);//cerrar conexion con el cliente 
    else
    {
        pfds[client].events = POLLIN;//volver a escuchar con el socket (ver cuando se sierra la conexion con el cliente)
        close_pfd(fd_file[client]);//cerrar el fd de archivo
        response.erase(client);
        fd_file.erase(client);
        cgiClients.erase(client);
        requests[client].reset();
        requests[client].last_conection_time();//guardar el tiempo de ultima conexion
    }
}

void    SocketManager::sendResponse()
{
    ssize_t send_size;

    for (std::map<int, int>::iterator it = fd_file.begin(); it != fd_file.end(); ++it)
    {
        std::cout << "socket " << it->first << " fd " << it->second << std::endl;
    }
    std::cout << "\033[31m" << " sendResponse " << "\033[0m" << std::endl;
    for (int client = listen_sockets; client < BACKLOG; client++)//recorre todos los sockets
    {
        if ((pfds[client].revents & POLLOUT) && !is_file(client) && fd_file.find(client) != fd_file.end() && pfds[client].fd != -1)//si algun socket tiene un revent de POLLOUT
        {
            if (response[client].getBytesRead() != 0 //no se ha leido nada
                && ((fd_file[client] != -1 && (pfds[fd_file[client]].revents == 0))  || response[client].getErrorCode() == INTERNAL_SERVER_ERROR) //el revent del fd de archivo esta en 0 o hay un error interno
                && (response[client].get_fileStat().st_size > BUFFER_SIZE || response[client].getPipeRes()))//se lee de una pipe o el tamaño del archivo es muy largo
            {
                response[client].setStringBuffer(make_chunked("", 0));
                response[client].setValread(0); 
            }
            if (!response[client].getStringBuffer().empty())
            {
                send_size = send(pfds[client].fd, response[client].getStringBuffer().c_str(), response[client].getStringBuffer().size(), 0);//enviar el buffer leido de archivo
                std::cout << "\033[36m" << response[client].getStringBuffer() << "\033[0m" << std::endl;
                response[client].setSendSize(send_size);
                if (response[client].get_fileStat().st_size > BUFFER_SIZE || response[client].getPipeRes())
                {
                    if (response[client].getValread() == 0)
                        managerFinishSend(client);
                }
                else
                    managerFinishSend(client);
                response[client].remove_sent_data(send_size);
            }
        }
    }
}

void    SocketManager::CommonGatewayInterface()
{
    pid_t pid_ret;
    int wstatus;

    std::cout << "\033[36m" << " CommonGatewayInterface " << "\033[0m" << std::endl;
    std::map<int, CGI>::iterator it = cgiClients.begin();
    while (it != cgiClients.end())
    {
        if (it->second.getPid() == -2)//si aun no se ha creado el proceso, solo se ha construido el objeto
        {
            //std::cout << it->first << "it->second.getPid()  " << it->second.getPid()  << std::endl;
            if (it->second.makeProcess())//cambia el pid del proceso al pid del proceso hijo
            {
                response[it->first].setErrorCode(INTERNAL_SERVER_ERROR);
                ErrorResponse(response[it->first], fd_file[it->first], it->first);
                cgiClients.erase(it);
            }
            response[it->first].setPipeRes(true);
        }
        else
        {
            pid_ret = waitpid(it->second.getPid(), &wstatus, WNOHANG);
            if (pid_ret == -1)//si hay error, devolver error 500
            {
                kill(cgiClients[it->first].getPid(), SIGINT);
                close(it->second.getFDread());
                close(it->second.getFDwrite());
                response[it->first].setErrorCode(INTERNAL_SERVER_ERROR);
                ErrorResponse(response[it->first], fd_file[it->first], it->first);
                cgiClients.erase(it);
            }
            else if (pid_ret > 0)//si el hijo ha  terminado
            {
                if (WIFSIGNALED(wstatus) || WEXITSTATUS(wstatus))
                {
                    kill(cgiClients[it->first].getPid(), SIGINT);
                    close(it->second.getFDread());
                    close(it->second.getFDwrite());
                    response[it->first].setErrorCode(INTERNAL_SERVER_ERROR);
                    ErrorResponse(response[it->first], fd_file[it->first], it->first);
                    cgiClients.erase(it);
                }
                else
                {
                    int pos_free = free_pos();
                    std::cout << "\033[35m" << " Child finish " << "\033[0m" << std::endl;
                    response[it->first].setFDpipe(it->second.getFDread(), it->second.getFDwrite());
                    pfds[pos_free].fd = it->second.getFDread();
                    pfds[pos_free].events = POLLIN;
                    pfds[it->first].events = POLLOUT;
                    fd_file[it->first] = pos_free;
                    cgiClients.erase(it);
                }
            }
            it++;
        }
    }
}

void    SocketManager::close_pfd(int pfd_free)
{
    int file_pos = -1;

    if (!is_file(pfd_free))
    {
        if (fd_file.find(pfd_free) != fd_file.end())
            file_pos = fd_file[pfd_free];
        if (cgiClients.find(pfd_free) != cgiClients.end())
        {
            if (cgiClients[pfd_free].getPid() != -2)
            {
                kill(cgiClients[pfd_free].getPid(), SIGINT);
                close(cgiClients[pfd_free].getFDread());//vervsi esto no da error
                close(cgiClients[pfd_free].getFDwrite());
            }
            cgiClients.erase(pfd_free);
        }
        requests.erase(pfd_free);
        response.erase(pfd_free);
        fd_file.erase(pfd_free);
        if (file_pos != -1)
        {
            if (pfds[file_pos].fd != -1)
            {
                close(pfds[file_pos].fd);
                pfds[file_pos].fd = -1;
                pfds[file_pos].events = 0;
                pfds[file_pos].revents = 0;
            }
        }   
        close(pfds[pfd_free].fd);//cerramos el socket
        pfds[pfd_free].fd = -1;
        pfds[pfd_free].events = 0;
        pfds[pfd_free].revents = 0;
    }
    else
    {
        if (pfd_free != -1 && pfds[pfd_free].fd != -1)
        {
            close(pfds[pfd_free].fd);
            pfds[pfd_free].fd = -1;
            pfds[pfd_free].events = 0;
            pfds[pfd_free].revents = 0;
        }
    }
}

std::string SocketManager::make_response_str(Response &response, std::string buffer)
{
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

std::string SocketManager::separateHeaders(std::string &buffer)
{
    size_t pos_end;
    std::string headers;

    pos_end = buffer.find("\r\n\r\n");
    if (pos_end == std::string::npos && pos_end != (buffer.size() - 4))
    {
        pos_end = buffer.find("\n\n");
        if (pos_end != std::string::npos && pos_end != (buffer.size() - 2))
        {
            headers = buffer.substr(0, pos_end);
            buffer.erase(0, (pos_end + 2));
        }
    }
    else if (pos_end != (buffer.size() - 4))
    {
        headers = buffer.substr(0, pos_end);
        buffer.erase(0, (pos_end + 4));
    }
    size_t pos = headers.find("Content-Length:");
    if (pos != std::string::npos)
    {
        pos_end = headers.find("\r\n", pos);
        if (pos_end == std::string::npos)
        {
            pos_end = headers.find("\n", pos);
            if (pos_end != std::string::npos)
                headers.erase(pos, (pos_end + 1));
        }
        else
            headers.erase(pos, (pos_end + 2));
    }
    return (headers);
}

std::string SocketManager::make_chunked_response(Response &response, std::string buffer, int valread)
{
    time_t rawtime;
    std::string day;
    struct tm *timeinfo;
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    const char* days[] = {"Mon", "Tues", "Weds", "Thur", "Fri", "Sat", "Sun"};

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::ostringstream str;
    std::string headers = separateHeaders(buffer);

    (void)valread;
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
        << "Transfer-Encoding: chunked" << "\r\n";
    if (!headers.empty())
        str << headers << "\r\n";
    str << "\r\n" << std::hex << buffer.size() << "\r\n" << buffer << "\r\n";
    return (str.str());
}

std::string SocketManager::make_chunked(std::string buffer, int valread)
{
    std::ostringstream str;
    if (valread == 0)
        str << std::hex << 0 << "\r\n\r\n";
    else if (!buffer.empty())
        str << std::hex << buffer.size() << "\r\n" << buffer << "\r\n";
    return (str.str());
}
