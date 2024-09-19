# include "../inc/Webserver.hpp"

SocketManager::SocketManager(){}

SocketManager::~SocketManager(){}

SocketManager::SocketManager(const SocketManager &other)
{
    *this = other;
}

SocketManager &SocketManager::operator=(const SocketManager &other)
{   
    return *this;
}

SocketManager::SocketManager(struct pollfd* pfds, std::vector<Server> &server)
{
    int sockfd;
    this->sock_num = 0;
    int ret;
    int flag = 0;
    int optval = 1;
    struct sockaddr_in  socket_addr;

    for (size_t i = 0; i < server.size(); ++i)
    {
        std::vector<struct sockaddr_in> addr_vect = server[i].getListen();
        for (size_t a = 0; a < addr_vect.size(); ++a)
        {
            if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1)
            {
                if ((ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) != -1)
                {
                    socket_addr = addr_vect[a];
                    if ((ret = bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr))) != -1)
                    {
                        if ((ret = listen(sockfd, BACKLOG)) != -1)
                        {
                            pfds[sock_num].fd = sockfd;
                            pfds[sock_num].events = POLLIN;
                            sock_num++;
                        }
                    }
                }
            }
            if (sockfd == -1 || ret == -1)
                std::cerr << "Error: Can't connect socket " << server[i].getServerName() << " to port." << std::endl;
            if (sock_num == BACKLOG - 1)
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

int SocketManager::getSockNum() const
{
    return (this->sock_num);
}

void    SocketManager::AcceptClient(struct pollfd* pfds, int ready)
{
    int new_sock;

    for (int i = 0; i < sock_num; i++)
    {
        if (sock_num == (BACKLOG - 1))
            std::cerr << "Error: Too many file descriptors in use." << std::endl;//que hacer si nuevo cliente no tiene espacio en pollfd???
        else if (pfds[i].revents & POLLIN)
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            if ((new_sock = accept(pfds[i].fd, (struct sockaddr *)&client_addr, &client_len)) != -1)
            {
                pfds[sock_num].fd = new_sock;
                pfds[sock_num].events = POLLIN;
                sock_num++;
            }
            else
                std::cerr << "Error: Accept client call." << std::endl;
        }
    }
}

void    SocketManager::recvRequest(struct pollfd* pfds, std::vector<Server> &server)
{
    int     valread;
    char    buffer[BUFFER_SIZE + 1] = {0};

    for (int i = listen_sockets; i <= sock_num; i++)//recorre todos los sockets
    {
        if (pfds[i].revents & POLLIN)//si algun socket tiene un revent de POLLIN
        {
            valread = recv(pfds[i].fd, buffer, BUFFER_SIZE, 0);//recibimos el mensaje de cliente
            requests[i] = Request(i, pfds[i].fd);//si se destruye SocketManager se destruye tambien Request?
            if (valread <= 0)//ha terminado de recibir el mensaje
            {
                pfds[i].events = POLLOUT;//cambiamos en event de socket al POLLOUT, porque la request ya ha llegado entera y tenemos que responder al cliente
                requests[i].validity();//parsear la request
                response[i] = requests[i].request_resolution(server);//parsear con los datos de conf de server, si hay algun error, va a crear una response de error
                pfds[sock_num].fd = response[i].open_file(sock_num);//abre el archivo a enviar y retorna el numero fd (comprobar si se ha abierto bien el archivo, si hay error, enviar respuesta de error)
                //hay que indicar en que posicion de pdfs esta el fd del archivo a enviarle.
                pfds[sock_num].events = POLLIN;//el evento para poder leer desde el archivo
                sock_num++;
            }
            else
            {
                if (!requests[i].split_request(buffer))//juntar los request y ver si body es mas largo de lo permitido. Si esta mal hay que indicar el _error_code para generar la respuesta de error
                {
                    pfds[i].events = POLLOUT;
                    response[i] = Response(requests[i]);//crrea la response de error
                    pfds[sock_num].fd = response[i].open_file(sock_num);//retorna fd de archivo de error
                    pfds[sock_num].events = POLLIN;
                    sock_num++;
                }
            }
            memset(buffer, 0, strlen(buffer));
        }
    }
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
            _pos_file_response = response[i].get_pos_file_response();//posicion en pfds donde esta guardado el fd del archivo a enviar
            valread = read(pfds[_pos_file_response].fd, buffer, BUFFER_SIZE);//leer del archivo a enviar BUFFER_SIZE bytes
            //cambiar la i por la posicion que tiene el archivo a enviar
            if (valread == -1)//leemos de archivo BUFFER_SIZE bytes, si da error hay que responder con error code 503
            {
                pfds[_pos_file_response].fd = open("/error/503.html", O_RDONLY);//abrir el archivo de error que se va a enviar y guardarlo en la posicion de pfds de este cliente
                pfds[_pos_file_response].events = POLLIN;
            }
            else
            {
                send(pfds[i].fd, buffer, strlen(buffer), 0);//enviar el buffer leido de archivo
                if (valread != BUFFER_SIZE)//significa que hemos llegado hasta el final del archivo
                {
                    pfds[i].events = POLLIN;//volver a escuchar con el socket (ver cuando se sierra la conexion con el cliente)
                    close(pfds[_pos_file_response].fd);//serrar el fd de archivo
                    sock_num--;//ver que hay que hacer con el pfds que se queda vacio al serrar el fd del archivo
                }
            }
        }
        memset(buffer, 0, strlen(buffer));
    }
}

/*
el socket del cliente se sierra cuando hay algun error o timeout. 
Si hay un error se manda el archivo de error, 
en el header se indica que se sierra la conexion con el socket.
VER EL RFC!!!
*/
