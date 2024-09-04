#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <map>
#include "Server.hpp"

class Server;

class Location
{
    public:
        std::vector<struct sockaddr_in>    listen;
        std::string    server_name;
        std::string    accept_method;
        std::map<int, std::string>    error_page;
        unsigned long long int    client_max_body_size;
        std::string    redirection;
        std::string    root;
        std::string    index;
        std::string    autoindex;
        //std::string    cgi;
    public:
        Location();
        ~Location();
        Location(const Location &);
        //Location(Server &);
        Location &operator=(const Location &);

        void    setErrorPage(std::vector<std::string> &words);
        void    setBodySize(std::vector<std::string> &words);
        void    setRoot(std::string);
        void    setAutoindex(std::string);
        void    setIndex(std::string);
        //void    setCGI(std::string);

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
};

#endif