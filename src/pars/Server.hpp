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
        std::vector<struct sockaddr_in> listen;      //es 80
        std::string server_name;                     //obligatorio
        std::string root;                            //obligatorio
        std::string index;                           //404
        std::string accept_method;                   //get
        std::string redirection;                     //no es obligatorio
        std::map<int, std::string>  error_page;      //obligatorio
        unsigned long long int  client_max_body_size;//0
        bool    autoindex;                           //bool off
        bool    cgi;                                 //bool off
        std::vector<Location*>  location;            //no es obligatorio
    public:
        Server();
        ~Server();
        Server(const Server &);
        Server &operator=(const Server &);

        void    setListen(std::string);
        void    setServerName(std::string);
        void    setRoot(std::string);
        void    setIndex(std::string);
        void    setAcceptMethod(std::string);
        void    setRedirection(std::string);
        void    setErrorPage(std::vector<std::string> &);
        void    setBodySize(std::vector<std::string> &);
        void    setAutoindex(std::string);
        void    setCGI(std::string);
        
        void    setUri(std::string, int);
        void    setLocation(std::vector<std::string> &);
        void    fillLocation();
        void    make_location();

        std::vector<struct sockaddr_in>    getListen() const;
        std::string    getServerName() const;
        std::string    getRoot() const;
        std::string    getIndex() const;
        std::string    getAcceptMethod() const;
        std::string    getRedirection() const;
        std::map<int, std::string>    getErrorPage() const;
        unsigned long long int    getBodySize() const;
        bool    getAutoindex() const;
        bool    getCGI() const;
        std::string     getUri(int) const;
        std::vector<Location*>  getLocation() const;
        
        void    printValuesServer() const;
};

#endif