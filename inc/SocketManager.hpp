# pragma once
# include "../inc/Webserver.hpp"

class CGI;

class SocketManager
{
    private:
        int sock_num;
        int listen_sockets;
        std::map<int, Request>  requests;
        std::map<int, Response> response;
        std::map<int, int>      fd_file;
        std::map<int, std::string> status_code;
        std::vector<struct sockaddr_in> open_addr;
        struct pollfd* pfds;
        std::map<int, CGI> cgiClients;
    public:
        SocketManager();
        ~SocketManager();
        SocketManager(const SocketManager &);
        SocketManager &operator=(const SocketManager &);
        SocketManager(struct pollfd*, std::vector<Server> &);

        int         getSockNum() const;
        void        acceptClient();
        void        recvRequest(std::vector<Server> &, int);
        void        readFile(int, int);
        void        reventPOLLIN(std::vector<Server> &);
        void        sendResponse();
        void        CommonGatewayInterface();
        int         connect_socket(struct sockaddr_in &);
        void        make_response(int);
        void        check_join(int, std::vector<Server> &, char *, int);
        void        ErrorResponse(Response &, int &, int);
        std::string make_response_str(Response &, std::string);
        int         is_file(int);
        int         check_revent(int);
        int         its_open(struct sockaddr_in &);
        std::string make_chunked_response(Response &, std::string);
        std::string make_chunked(std::string, int);
        int         deleteMethod(int);
        void        managerFinishSend(int);
        std::string separateHeaders(std::string &);
        void        close_pfd(int);
        int         free_pos();
        int         check_headers(std::string &);
        int         is_name(std::string &);
        int         is_value(std::string &);
        void        clearObject();
};
