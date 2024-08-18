#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>

#define PORT    8080
#define IP      0x7f000001 //127.0.0.1 en unsigned long con la ordenación de bytes
#define BACKLOG 128

int main()
{
    struct sockaddr_in  socket_addr;
    struct pollfd       fds[BACKLOG];
    int                 client_size;
    int                 pollfds;
    int                 sockfd;
    int                 new_sockfd;
    int                 recv_ret;
    int                 optval = 1;
    int addrlen = sizeof(socket_addr);
    char buf[1024] = {0};
    const char *response = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: 52\r\n"
                       "\r\n"
                       "<html><body><h1>Hello, wowoWorld!</h1></body></html>";

    client_size = 1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }
    
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(PORT);
    socket_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    for (int i = 1; i < BACKLOG; i++)
        fds[i].fd = -1;
    while (true)
    {
        if ((pollfds = poll(fds, BACKLOG, -1)) == -1)
            std::cout << strerror(errno) << std::endl;
        if (fds[0].revents & POLLIN)
        {
            if (client_size < BACKLOG)
            {
                if ((new_sockfd = accept(fds[0].fd, (struct sockaddr *)&socket_addr, (socklen_t*)&addrlen)) < 0)
                {
                    std::cout << strerror(errno) << std::endl;
                    exit(1);
                }
                fds[client_size].fd = new_sockfd;
                fds[client_size].events = POLLIN;
                std::cout << fds[client_size].fd << "fd del cliente\n"; 
                client_size++;
            }
            else
                std::cout << "error" << std::endl;//se ha pasado el tamaño de pollfd
        }
        for (int i = 1; i < BACKLOG; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                std::cout << i << " = i " << fds[i].fd << " fd del cliente respuesta\n"; 
                    recv_ret = recv(fds[i].fd, buf, sizeof(buf), 0);
                    if (recv_ret == -1)
                    {
                        std::cout << strerror(errno) << std::endl;
                        exit(1);
                    }
                    if (recv_ret == 0)
                        close(fds[i].fd);
                    else
                    {
                        if (send(fds[i].fd, response, strlen(response), 0) < 0)
                        {
                            std::cout << strerror(errno) << std::endl;
                            exit(1);
                        }
                    }
            }
        }
    }
    close(fds[0].fd);
}
