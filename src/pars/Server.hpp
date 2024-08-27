#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>

class Server
{
    private:
        std::string    listen;
        std::string    server_name;
        std::string    accept_method;
        std::string    error_page;
        std::string    client_max_body_size;
        std::string    redirection;
        std::string    root;
        std::string    autoindex;
        std::string    index;
        std::string    cgi;
    public:
        Server();
        ~Server();
        Server(const Server &);
        Server &operator=(const Server &);

        void    setListen(std::string);
        void    setServerName(std::string);
        void    setAcceptMethod(std::string);
        void    setErrorPage(std::string);
        void    setBodySize(std::string);
        void    setRedirection(std::string);
        void    setRoot(std::string);
        void    setAutoindex(std::string);
        void    setIndex(std::string);
        void    setCGI(std::string);

        std::string    getServerName() const;
        std::string    getListen() const;
        std::string    getAcceptMethod() const;
        std::string    getErrorPage() const;
        std::string    getBodySize() const;
        std::string    getRedirection() const;
        std::string    getRoot() const;
        std::string    getAutoindex() const;
        std::string    getIndex() const;
        std::string    getCGI() const;
};

#endif