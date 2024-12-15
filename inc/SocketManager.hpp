# pragma once
# include "../inc/Webserver.hpp"

class CGI;

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
        struct pollfd* pfds;
        
        //nuevo
        std::map<int, CGI> cgiClients;//son los que tienen una request con cgi

        SocketManager();
    public:
        ~SocketManager();
        SocketManager(const SocketManager &);
        SocketManager &operator=(const SocketManager &);
        SocketManager(struct pollfd*, std::vector<Server> &);

        int     getSockNum() const;

        void    acceptClient();
        void    recvRequest(std::vector<Server> &, int);
        void    readFile(int, int);
        void    reventPOLLIN(std::vector<Server> &);
        void    sendResponse();
        void    CommonGatewayInterface();

        int     connect_socket(struct sockaddr_in &);
        void    make_response(int);
        void    check_join(int, std::vector<Server> &, char *, int);
        void    close_move_pfd(int);
        void    ErrorResponse(Response &, int &, int);
        std::string make_response_str(Response &, std::string);
        int is_file(int);
        void    check_revent(int);
        int its_open(struct sockaddr_in &);
        std::string make_chunked_response(Response &, std::string, int);
        std::string make_chunked(std::string, int);
        int deleteMethod(int);
        void    managerFinishSend(int);
        std::string separateHeaders(std::string &);
        std::string ContentTipe(std::string);
        void    move_pfd(int);
        void    close_pfd(int);
        int free_pos();
        int check_headers(std::string &);
        int is_name(std::string &);
        int is_value(std::string &);

        void    priint(int);
};
