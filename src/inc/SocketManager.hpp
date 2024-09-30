# pragma once
# ifndef SOCKET_MANAGET_HPP
# define SOCKET_MANAGER_HPP
# include "../inc/Webserver.hpp"

class SocketManager
{
    private:
        int sock_num;//socket de escucha + socket de clientes
        int listen_sockets;//socket de escucha
        std::map<int, Request>  requests;//posicion del cliente en pfds y su correspondiente request
        std::map<int, Response> response;//posicion del cliente en pfds y su correspondiente response
        std::map<int, int>      fd_file;//posicion del cliente en pfds y su correspondiente fd de archivo responder
        std::map<int, std::string> status_code;///codigo de respuesta y el texto que va en el response line

        SocketManager();
    public:
        ~SocketManager();
        SocketManager(const SocketManager &);
        SocketManager &operator=(const SocketManager &);
        SocketManager(struct pollfd*, std::vector<Server> &);

        int     getSockNum() const;

        void    acceptClient(struct pollfd*, int);
        void    recvRequest(struct pollfd*, std::vector<Server> &);
        void    sendResponse(struct pollfd*);

        int     find_free_pos_socket(struct pollfd*, int);
        int     free_pfds_fd(struct pollfd*, std::map<int, Response> &, int);
        int     connect_socket(struct pollfd*, struct sockaddr_in &);
        void    make_response(int, struct pollfd*, std::vector<Server> &);
        void    check_join(int, struct pollfd*, std::vector<Server> &, char *, int);
        void    close_move_pfd(struct pollfd*, int);
        void    sendErrorResponse(struct pollfd*, int, int);
};

# endif