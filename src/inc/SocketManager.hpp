# pragma once
# ifndef SOCKET_MANAGET_HPP
# define SOCKET_MANAGER_HPP
# include "../inc/Webserver.hpp"

class SocketManager
{
    private:
        int sock_num;//socket de escucha + socket de clientes
        int listen_sockets;//socket de escucha
        
        SocketManager();
    public:
        ~SocketManager();
        SocketManager(const SocketManager &);
        SocketManager &operator=(const SocketManager &);
        SocketManager(struct pollfd*, std::vector<Server> &);

        void    error(int);

        int getSockNum() const;
        void    AcceptClient(struct pollfd*, int);
        void    recvRequest(struct pollfd*, std::vector<Request> &);
};

# endif