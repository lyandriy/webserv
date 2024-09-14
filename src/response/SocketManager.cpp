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

SocketManager::SocketManager(struct pollfd* fds, std::vector<Server> &server)
{
    int sockfd;
    this->sock_num = 0;
    int ret;
    int optval = 1;
    struct sockaddr_in  socket_addr;

    for (size_t i = 0; i < server.size(); ++i)
    {
        std::vector<struct sockaddr_in> addr_vect = server[i].getListen();
        for (size_t a = 0; a < addr_vect.size(); ++a)
        {
            sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            error(sockfd);
            ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
            error(ret);
            socket_addr = addr_vect[a];
            ret = bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr));
            error(ret);
            ret = listen(sockfd, BACKLOG);
            error(ret);
            fds[sock_num].fd = sockfd;
            fds[sock_num].events = POLLIN;
            sock_num++;
            if (sock_num == BACKLOG - 1)
            {
                std::cerr << "Error: Too many ports in use" << std::endl;
	            exit(EXIT_FAILURE);
            }
        }
    }
    for (; sock_num < BACKLOG; sock_num++)
        fds[sock_num].fd = -1;
}

void SocketManager::error(int ret)
{
    if (ret == -1)
	{
        std::cerr << strerror(errno) << std::endl;
	    exit(EXIT_FAILURE);
	}
}

int SocketManager::getSockNum() const
{
    return (this->sock_num);
}