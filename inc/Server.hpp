# pragma once
#include "Webserver.hpp"

class Location;

class Server
{
    private:
        std::vector<struct sockaddr_in> listen;
        std::string server_name;
        std::string root;
        std::string index;
        httpMethods accept_method;
        std::map<int, std::string>  error_page;
        int  client_max_body_size;
        int    autoindex;
        int    cgi;
        std::vector<Location>  location;

        void    setListen(std::string);
        void    setServerName(std::string);
        void    setRoot(std::string);
        void    setIndex(std::string);
        void    setAcceptMethod(std::vector<std::string> &);
        void    setErrorPage(std::vector<std::string> &);
        void    setBodySize(std::vector<std::string> &);
        void    setCGI(std::string);
        
        void    setAutoindex(std::string);
        void    setUri(std::string, int);
        void    setLocation(std::vector<std::string> &);
        void    fillLocation();
        void    make_location();

        friend class Parser;
    public:
        Server();
        ~Server();
        Server(const Server &);
        Server(int);
        Server &operator=(const Server &);

        std::vector<struct sockaddr_in> getListen() const;
        std::string                     getServerName() const;
        std::string                     getRoot() const;
        std::string                     getIndex() const;
        httpMethods                     getAcceptMethod() const;
        std::map<int, std::string>      getErrorPage() const;
        int                             getBodySize() const;
        int                             getAutoindex() const;
        int                             getCGI() const;
        std::string                     getUri(int) const;
        std::vector<Location>           getLocation() const;

};