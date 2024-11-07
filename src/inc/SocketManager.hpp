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
        std::vector<struct sockaddr_in> open_addr;

        SocketManager();
    public:
        ~SocketManager();
        SocketManager(const SocketManager &);
        SocketManager &operator=(const SocketManager &);
        SocketManager(struct pollfd*, std::vector<Server> &);

        int     getSockNum() const;

        void    acceptClient(struct pollfd*);
        void    recvRequest(struct pollfd*, std::vector<Server> &, int);
        void    readFile(struct pollfd*, int, int);
        void    reventPOLLIN(struct pollfd*, std::vector<Server> &);
        void    sendResponse(struct pollfd*);

        int     connect_socket(struct pollfd*, struct sockaddr_in &);
        void    make_response(int, struct pollfd*);
        void    check_join(int, struct pollfd*, std::vector<Server> &, char *, int);
        void    close_move_pfd(struct pollfd*, int);
        std::string    ErrorResponse();
        std::string make_response_str(Response &, std::string);
        int is_file(int);
        void    check_revent(struct pollfd*, int);
        int its_open(struct sockaddr_in &);
};

# endif