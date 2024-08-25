#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Server.hpp"

class Server;

class Parser
{
    private:
        std::ifstream   in_file;
        Server          *server;
        //Location        *location;
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