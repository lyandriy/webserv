# pragma once
#include "Webserver.hpp"

class Server;

class Location
{
    public:
        std::vector<struct sockaddr_in> listen;
        std::string server_name;
        std::string root;
        std::string index;
        httpMethods accept_method;
        std::string redirection;
        std::map<int, std::string>  error_page;
        int  client_max_body_size;
        int    autoindex;
        int    cgi;
        std::string locationUri;   
    public:
        Location();
        ~Location();
        Location(const Location &);
        Location &operator=(const Location &);

        void    setListen(std::string);
        void    setServerName(std::string);
        void    setRoot(std::string);
        void    setIndex(std::string);
        void    setAcceptMethod(std::vector<std::string> &);
        void    setRedirection(std::string);
        void    setErrorPage(std::vector<std::string> &);
        void    setBodySize(std::vector<std::string> &);
        void    setAutoindex(std::string);
        void    setCGI(std::string);
        void    setUri(std::string);

        std::vector<struct sockaddr_in> getListen() const;
        std::string getServerName() const;
        std::string getRoot() const;
        std::string getIndex() const;
        httpMethods getAcceptMethod() const;
        std::string getRedirection() const;
        std::map<int, std::string>  getErrorPage() const;
        int  getBodySize() const;
        int    getAutoindex() const;
        int    getCGI() const;
        std::string getUri() const;

        Location    clone() const;
};