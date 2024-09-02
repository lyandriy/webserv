#include "Server.hpp"

Server::Server(){
    std::cout << "constructor de server" << std::endl;
}

Server::~Server(){}

Server::Server(const Server &other){
    *this = other;
}

Server &Server::operator=(const Server &other){return *this;}

struct sockaddr_in  Server::make_socaddr(std::string addr, int port)
{
    struct sockaddr_in socket_addr;

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = INADDR_ANY;

    return (socket_addr);
}

void    Server::setListen(std::vector<std::string> &words)
{
    std::string str_number;
    int port;
    std::string str_port;
    int i = 0;
    struct sockaddr_in socket_addr;

    while (isdigit(words[1][i]) || words[1][i])
    {
        std::cout << "sera aqui" << std::endl;
        str_number += words[1][i];
        i++;
    }
    if (words[1][i] == '.')
    {
        while (words[1][i] || words[1][i] != ':')
        {
            str_number += words[1][i];
            i++;
        }
        if (words[1][i] != ':')//cuando hay solo el numero IP
        {
            listen.push_back(make_socaddr(str_number, 80));
            str_number.clear();
        }
        if (words[1][i] == ':')
        {
            i++;
            while (words[1][i])
            {
                std::cout << "sera aqui2" << std::endl;
                str_port += words[1][i];
                i++;
            }
            port = atoi(str_port.c_str());
            str_port.clear();
            listen.push_back(make_socaddr(str_number, port));
        }
    }
    else if (!str_number.empty())//cuando hay solo puerto
    {
        port = atoi(str_number.c_str());
        std::cout << "port numero " << port << std::endl;
        str_number.clear();
        listen.push_back(make_socaddr("0.0.0.0", port));
    }
}

void    Server::setServerName(std::string server_name)
{
    this->server_name = server_name; 
}

void    Server::setAcceptMethod(std::string accept_method)
{
    this->accept_method = accept_method;
}

void    Server::setErrorPage(std::vector<std::string> &words)
{
    int a = 1;
    std::string str_number;
    int int_number;

    for (a = 1; a < (words.size() - 1); a++)
    {
        for (int i = 0; (isdigit(words[a][i]) && i < words[a].size()); i++)
            str_number += words[a][i];
        int_number = atoi(str_number.c_str());
        this->error_page[int_number] = words[words.size() - 1];
    }
}

void    Server::setBodySize(std::vector<std::string> &words)
{
    std::string str_number;
    unsigned long long int  int_number;
    int i = 0;

    while (isdigit(words[1][i]) && i < words[1].size())
    {
        str_number += words[1][i];
        i++;
    }
    if (!str_number.empty())
    {
        int_number = atol(str_number.c_str());
        if (words[0][i] == 'M' && !words[0][i++])
            int_number *= 1048576;
        else if (words[0][i] == 'k' && !words[0][i++])
            int_number *= 1024;
        else if (words[0][i] == 'G' && !words[0][i++])
            int_number *= 1073741824;
        this->client_max_body_size = int_number;
    }
}

void    Server::setRedirection(std::string redirection)
{
    this->redirection = redirection;
}

void    Server::setRoot(std::string root)
{
    this->root = root;
}

void    Server::setAutoindex(std::string autoindex)
{
    this->autoindex = autoindex;
}

void    Server::setIndex(std::string index)
{
    this->index = index;
}

/*void    Server::setCGI(std::string cgi)
{
    this->cgi = cgi;
}*/

std::string    Server::getServerName() const
{
    return(this->server_name);
}

std::vector<struct sockaddr_in>    Server::getListen() const
{
    return(this->listen);
}

std::string    Server::getAcceptMethod() const
{
    return(this->accept_method);
}

std::map<int, std::string>    Server::getErrorPage() const
{
    return(this->error_page);
}

unsigned long long int    Server::getBodySize() const
{
    return(this->client_max_body_size);
}

std::string    Server::getRedirection() const
{
    return(this->redirection);
}

std::string    Server::getRoot() const
{
    return(this->root);
}

std::string    Server::getAutoindex() const
{
    return(this->autoindex);
}

std::string    Server::getIndex() const
{
    return(this->index);
}

/*std::string    Server::getCGI() const
{
    return(this->cgi);
}*/

void    Server::createLocation()
{
    this->location.push_back(new Location(*this));
}
void    Server::setLocation(std::vector<std::string> &words)
{
    Location*   back = location.back();
    if (words[0] == "error_page")
        back->setErrorPage(words);
    else if (words[0] == "index")
        back->setIndex(words[1]);
    else if (words[0] == "client_max_body_size")
        back->setBodySize(words);
    else if (words[0] == "root")
        back->setRoot(words[1]);
    else if (words[0] == "autoindex")
        back->setAutoindex(words[1]);
   /* else if (words[0] == "cgi")
        back->setCGI(words[1]);*/
    else
        throw std::runtime_error("error_setLocation");
}
