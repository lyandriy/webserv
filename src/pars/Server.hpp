#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include "Location.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>

class Location;

class Server
{
    private:
        std::vector<struct sockaddr_in>    listen;
        std::string    server_name;
        std::string    accept_method;
        std::map<int, std::string>    error_page;
        unsigned long long int    client_max_body_size;
        std::string    redirection;
        std::string    root;
        std::string    autoindex;
        std::string    index;
        //std::string    cgi;
        std::vector<Location*>   location;
        int location_size;
    public:
        Server();
        ~Server();
        Server(const Server &);
        Server &operator=(const Server &);

        void    setListen(std::string);
        void    setServerName(std::string);
        void    setAcceptMethod(std::string);
        void    setErrorPage(std::vector<std::string> &);
        void    setBodySize(std::vector<std::string> &);
        void    setRedirection(std::string);
        void    setRoot(std::string);
        void    setAutoindex(std::string);
        void    setIndex(std::string);
        //void    setCGI(std::string);
        void    createLocation();
        void    setLocation(std::vector<std::string> &);
        void    fillLocation();
        void    make_location();

        std::string    getServerName() const;
        std::vector<struct sockaddr_in>    getListen() const;
        std::string    getAcceptMethod() const;
        std::map<int, std::string>    getErrorPage() const;
        unsigned long long int    getBodySize() const;
        std::string    getRedirection() const;
        std::string    getRoot() const;
        std::string    getAutoindex() const;
        std::string    getIndex() const;
        //std::string    getCGI() const;

        struct sockaddr_in  make_socaddr(std::string, int);
        void printValuesServer() const;
};

#endif