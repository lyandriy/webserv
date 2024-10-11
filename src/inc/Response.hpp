# pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "Webserver.hpp"

class Request;

class Response
{
    private:
        struct sockaddr_in listen;//el puerto por el que van a comunicarse
        std::string host;//es el server_name
        std::string root;//la ruta donde estan los archivos
        std::string uri;//lo que va despues de server_name(host), se una el root y uri y se busca alli
        std::string redirection;//redirecciona una ruta a otra
        std::string index;//de envia este archivo en caso si indican solo la ruta sin el archivo
        bool    autoindex;//en caso que no esta indes, se manda el listado de archivos en la ruta
        std::string accept_method;//mettodo get, post, delete
        std::map<int, std::string>  error_page;//paginas de error que se pueden usar
        int error_code;
        
        bool    cgi;//on o off
        std::string protocol;
        std::string help_message;
        std::map<std::string, std::string>  headers;
        int _fd_socket;
	    int _pos_socket;//posicion del socket en pfds
        int _pos_file_response;//posicion del archivo que se va a responder en pfds
        struct stat fileStat;//informacion sobre el archivo
        std::string connection_val;
    public:
        Response();
        ~Response();
        Response &operator=(const Response &);
        Response(const Response &);
        Response(const Location &, Request &);
        Response(const Server &, Request &);
        Response(Request &);


        struct sockaddr_in getListen() const;
        std::string getHost() const;
        std::string getRoot() const;
        std::string getURI() const;
        std::string getRedirection() const;
        std::string getIndex() const;
        bool getAutoindex() const;
        std::string getAcceptMethod() const;
        std::map<int, std::string> getErrorPage() const;
        int getErrorCode() const;
        bool getCGI() const;
        std::string getProtocol() const;
        std::string getHelpMessage() const;
        std::map<std::string, std::string>  getHeaders() const;
        int get_fd_socket() const;
        int get_pos_socket() const;
        int get_pos_file_response() const;
        struct stat get_fileStat() const;
        std::string    getConnectionVal() const;

        void setListen(struct sockaddr_in);
        void setHost(std::string);
        void setRoot(std::string);
        void setURI(std::string);
        void setRedirection(std::string);
        void setIndex(std::string);
        void setAutoindex(bool);
        void setAcceptMethod(std::string);
        void setErrorPage(std::map<int, std::string>);
        void setErrorCode(int);
        void setCGI(bool);
        void setProtocol(std::string);
        void setHelpMessage(std::string);
        void setHeaders(std::map<std::string, std::string>);
        void setConnectionVal(std::string);

        int     open_file(int);
        void    err(int, std::string);


        void print_full_info();
};

#endif