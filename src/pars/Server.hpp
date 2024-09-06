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
        std::vector<struct sockaddr_in>    listen;//tiene que haber si no, es un 80
        std::string    server_name;//tiene que haber
        std::string    accept_method;//tiene que haber
        std::map<int, std::string>    error_page;//tiene que haber
        unsigned long long int    client_max_body_size;//0
        std::string    redirection;
        std::string    root;//tiene que haber
        bool    autoindex;//bool off
        std::string    index;//404
        bool    cgi;//bool off
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
        void    setCGI(std::string);
        void    setUri(std::string);
        void    setLocation(std::vector<std::string> &, int);
        void    fillLocation();
        void    make_location();

        std::string    getServerName() const;
        std::vector<struct sockaddr_in>    getListen() const;
        std::string    getAcceptMethod() const;
        std::map<int, std::string>    getErrorPage() const;
        unsigned long long int    getBodySize() const;
        std::string    getRedirection() const;
        std::string    getRoot() const;
        bool    getAutoindex() const;
        std::string    getIndex() const;
        bool    getCGI() const;
        std::string     getUri() const;

        void printValuesServer() const;
};

#endif