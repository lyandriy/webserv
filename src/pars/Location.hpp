#include <iostream>
#include "Server.hpp"

class Location : public Server
{
    private:
        std::string    listen;
        std::string    server_name;
        std::string    accept_method;
        std::string    error_page;
        std::string    client_max_body_size;
        std::string    redirection;
        std::string    root;
        std::string    autoindex;
        std::string    index;
        std::string    cgi;
    public:
        Location();
        ~Location();
        Location(const Location &);
        Location &operator=(const Location &);
};