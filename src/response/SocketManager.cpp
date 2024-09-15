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

void    SocketManager::recvRequest(struct pollfd* pfds , std::vector<Request>  &requests)
{
    int     valread;
    char    buffer[BUFFER_SIZE + 1] = {0};

    for (int i = listen_sockets; i <= sock_num; i++)
    {
        if (pfds[i].revents & POLLIN)
        {
            valread = recv(pfds[i].fd, buffer, BUFFER_SIZE, 0);
            if (valread <= 0)//ha terminado de recibir el mensaje
            {
                //creo que hay que crear un nuevo socket para response
                //ya se puede hacer el parseo de reques
            }
            else
                //para saber que parte del buffer corresponde a que cliente se puede comparar sockaddr con getsockname
                //aun no ha recibido todo el mensaje
                //ir comprobando el tamaño de body

        }
    }
}
