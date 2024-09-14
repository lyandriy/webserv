# pragma once
# ifndef SOCKET_MANAGET_HPP
# define SOCKET_MANAGER_HPP
# include "../inc/Webserver.hpp"

class SocketManager
{
    private:
        int sock_num;
        
        SocketManager();
    public:
        ~SocketManager();
        SocketManager(const SocketManager &);
        SocketManager &operator=(const SocketManager &);
        SocketManager(struct pollfd*, std::vector<Server> &);

        void    error(int);

        int getSockNum() const;
};

# endif