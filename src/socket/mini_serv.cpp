#include "webserv.hpp"

int main()
{
    struct sockaddr_in  socket_addr;
    struct pollfd       fds[BACKLOG];
    int                 pollfds;
    int                 sockfd;
    int                 new_sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        std::cout << strerror(errno) << std::endl;
    
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(PORT);
    socket_addr.sin_addr.s_addr = IP;

    if (bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
        std::cout << strerror(errno) << std::endl;
    if (listen(sockfd, BACKLOG) == -1)
        std::cout << strerror(errno) << std::endl;
    fds[0].fd = sockfd;
    fds[0].events = POLLIN | POLLPRI;
    for (int i = 1; i < BACKLOG; i++)
    {
        if (i == sockfd)
            i++;
        fds[i].fd = i + 2; 
        fds[i].events = POLLIN;
    }
    while (true)
    {
        if ((pollfds = poll(fds, BACKLOG, -1)) = -1)
            std::cout << strerror(errno) << std::endl;
        for (int i = 0; i < pollfds; i++)
        {
            
        }
    }
}