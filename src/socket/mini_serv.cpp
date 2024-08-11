#include "webserv.hpp"

int main()
{
    struct sockaddr_in  socket_addr;
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
    
}