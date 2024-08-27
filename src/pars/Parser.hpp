#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Server.hpp"
#include "Location.hpp"

class Server;

class Location;

class Parser
{
    private:
        std::ifstream           in_file;
        std::vector<Server*>     server;
        std::vector<Location*>   location;
        int                     server_size;
        int                     location_size;
        Parser();
    public:
        Parser(const std::string);
        ~Parser();

        void    conf_file();
        void    split(std::string &, std::vector<std::string> &);
        void    server_pars();
        void    key_words_server(std::vector<std::string> &, int);
        void    key_words_location(std::vector<std::string> &, int);
        int    listen(std::vector<std::string> &, int &);
        int    server_name(std::vector<std::string> &, int &);
        int    accept_method(std::vector<std::string> &, int &);
        int    error_page(std::vector<std::string> &, int &);
        int    index(std::vector<std::string> &, int &);
        int    client_max_body_size(std::vector<std::string> &, int &);
        int    redirection(std::vector<std::string> &, int &);
        int    root(std::vector<std::string> &, int &);
        int    autoindex(std::vector<std::string> &, int &);
        int    cgi(std::vector<std::string> &, int &);
        void    location_key(std::vector<std::string> &);
};

#endif