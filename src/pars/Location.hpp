#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <map>
#include "Server.hpp"
#include <arpa/inet.h>

class Server;

class Location
{
    public:
        std::vector<struct sockaddr_in>    listen;//si no indican el puerto, es 80
        std::string    server_name;
        std::string    accept_method;
        std::map<int, std::string>    error_page;
        unsigned long long int    client_max_body_size;
        std::string    redirection;
        std::string    root;
        std::string    index;
        bool    autoindex;
        bool     cgi;
        std::string     locationUri;
    public:
        Location();
        ~Location();
        Location(const Location &);
        Location &operator=(const Location &);

        void    setErrorPage(std::vector<std::string> &words);
        void    setBodySize(std::vector<std::string> &words);
        void    setRoot(std::string);
        void    setAutoindex(std::string);
        void    setIndex(std::string);
        void    setCGI(std::string);
        void    setUri(std::string);

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
        void printValues() const;
};

#endif