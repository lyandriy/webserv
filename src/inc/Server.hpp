# pragma once
#ifndef SERVER_HPP
#define SERVER_HPP
#include "Webserver.hpp"

class Location;

class Server
{
    private:
        std::vector<struct sockaddr_in> listen;      //es 80
        std::string server_name;                     //obligatorio
        std::string root;                            //obligatorio
        std::string index;                           //404
        httpMethods accept_method;                   //get
        std::pair<int, std::string> redirection;     //no es obligatorio
        std::map<int, std::string>  error_page;      //obligatorio
        int  client_max_body_size;//0
        int    autoindex;                           //bool off
        int    cgi;                                 //bool off
        std::vector<Location>  location;            //no es obligatorio

        void    setListen(std::string);
        void    setServerName(std::string);
        void    setRoot(std::string);
        void    setIndex(std::string);
        void    setAcceptMethod(std::vector<std::string> &);
        void    setRedirection(std::vector<std::string> &);
        void    setErrorPage(std::vector<std::string> &);
        void    setBodySize(std::vector<std::string> &);
        void    setAutoindex(std::string);
        void    setCGI(std::string);
        
        void    setUri(std::string, int);
        void    setLocation(std::vector<std::string> &);
        void    fillLocation();
        void    make_location();

        friend class Parser;
    public:
        Server();
        ~Server();
        Server(const Server &);
        Server &operator=(const Server &);

        std::vector<struct sockaddr_in>    getListen() const;
        std::string    getServerName() const;
        std::string    getRoot() const;
        std::string    getIndex() const;
        httpMethods    getAcceptMethod() const;
        std::pair<int, std::string>    getRedirection() const;
        std::map<int, std::string>    getErrorPage() const;
        int    getBodySize() const;
        int    getAutoindex() const;
        int    getCGI() const;
        std::string     getUri(int) const;
        std::vector<Location>  getLocation() const;
        
        void    printValuesServer() const;
};

#endif