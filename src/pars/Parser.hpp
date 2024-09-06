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
        bool inLocationBlock;
        std::ifstream           in_file;
        std::vector<Server*>     server;
        std::vector<std::string>    words;
        int                     server_size;
        int                     location_size;
        std::string             line;
        Parser();
    public:
        Parser(const std::string);
        ~Parser();

        void    conf_file();
        void    split(std::string &);
        void    server_pars();
        void    key_words_server();
        void    key_words_location();
        int     listen();
        int     server_name();
        int     accept_method();
        int     error_page();
        int     index();
        int     client_max_body_size();
        int     redirection();
        int     root();
        int     autoindex();
        int     cgi();
        void    location_key();
        bool    valid_path();
        int     error_code(std::string);
        int     is_number(std::string);
        void    IaMServer();
        void    check_content();
};

#endif