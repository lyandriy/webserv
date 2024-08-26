#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Server.hpp"
#include "Location.hpp"

class Server;

class Parser
{
    private:
        void (Parser::*methods[])() = {
            &Parser::listen,
            &Parser::server_name,
            &Parser::accept_method,
            &Parser::error_page,
            &Parser::index,
            &Parser::client_max_body_size,
            &Parser::redirection,
            &Parser::root,
            &Parser::autoindex,
            &Parser::cgi
            };
        std::string             key[10] = {"listen", "server_name", "accept_method", "error_page", "index",
                                            "client_max_body_size", "redirection", "root", "autoindex", "cgi"};
        std::ifstream           in_file;
        std::vector<Server>     server;
        int                     server_size;
        int                     location_size;
        Parser();
    public:
        Parser(const std::string);
        ~Parser();

        void    conf_file();
        void    split(std::string &, std::vector<std::string> &);
        void    server_pars();
        void    key_words(std::vector<std::string> &);
        void    listen();
        void    server_name();
        void    accept_method();
        void    error_page();
        void    index();
        void    client_max_body_size();
        void    redirection();
        void    root();
        void    autoindex();
        void    cgi();
};

#endif