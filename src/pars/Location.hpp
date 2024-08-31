#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include "Server.hpp"

class Location
{
    private:
        std::string    listen;
        std::string    server_name;
        std::string    accept_method;
        std::map<int, std::string>    error_page;
        unsigned long long int    client_max_body_size;
        std::string    redirection;
        std::string    root;
        std::string    autoindex;
        std::string    index;
        std::string    cgi;
    public:
        Location();
        ~Location();
        Location(const Location &);
        Location &operator=(const Location &);
        Location(Server*);

        void    setErrorPage(std::string);
        void    setBodySize(std::string);
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