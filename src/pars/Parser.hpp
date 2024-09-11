# pragma once
#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Server.hpp"
#include <algorithm>

class Server;

class Parser
{
    private:
        bool inServerBlock;
        std::ifstream           in_file;
        std::vector<Server*>     server;
        std::vector<std::string>    words;
        int                     server_size;
        int                     location_size;
        std::string             line;

        Parser();

        void    split(std::string &);
        bool    valid_path();
        int     is_number(std::string);
        int     error_code(std::string);
        void    check_content();
        
        int     listen();
        int     server_name();
        int     root();
        int     redirection();
        int     index();
        int     accept_method();
        int     error_page();
        int     client_max_body_size();
        int     autoindex();
        int     cgi();

        

        void    key_words_location();
        void    location_pars();

        void    key_words_server();
        void    server_pars();

        void    IaMServer();
    public:
        Parser(const std::string);
        ~Parser();

        std::vector<Server*>    conf_file();
        
};

#endif