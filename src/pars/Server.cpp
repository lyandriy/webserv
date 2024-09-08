#include "Server.hpp"

Server::Server(){
    accept_method = "GET";
    client_max_body_size = 0;
    cgi = false;
    autoindex = false;
    index = "/error/404.html";
}

Server::~Server(){}

Server::Server(const Server &other){
    *this = other;
}

Server &Server::operator=(const Server &other){return *this;}

void    Server::setListen(std::string word)
{
    int port;
    struct sockaddr_in socket_addr;

    port = atoi(word.c_str());
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = INADDR_ANY;
    listen.push_back(socket_addr);
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
        str_number.clear();
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
        if (words[1][i] == 'M')
            int_number *= 1048576;
        else if (words[1][i] == 'k')
            int_number *= 1024;
        else if (words[1][i] == 'G')
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
    if (autoindex == "on")
        this->autoindex = true;
    else if (autoindex == "off")
        this->autoindex = false;
}

void    Server::setIndex(std::string index)
{
    this->index = index;
}

void    Server::setCGI(std::string cgi)
{
    if (cgi == "on")
        this->cgi = true;
    else if (cgi == "off")
        this->cgi = false;
}

void    Server::setUri(std::string Uri)
{
    this->location.back()->setUri(Uri);
}

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

bool    Server::getAutoindex() const
{
    return(this->autoindex);
}

std::string    Server::getIndex() const
{
    return(this->index);
}

bool   Server::getCGI() const
{
    return(this->cgi);
}

std::string    Server::getUri() const
{
    return (this->location.back()->getUri());
}

void    Server::fillLocation()
{
    for (std::vector<Location*>::iterator itl = location.begin(); itl != location.end(); ++itl)
    {
        (*itl)->listen = this->listen;
        (*itl)->server_name = this->server_name;
        (*itl)->accept_method = this->accept_method;
        (*itl)->redirection = this->redirection;
        (*itl)->cgi = this->cgi;
        if ((*itl)->root.empty())
            (*itl)->root = this->root;
        for (std::map<int, std::string>::iterator it = this->error_page.begin(); it != this->error_page.end(); ++it)
        {
            if ((*itl)->error_page.find(it->first) == (*itl)->error_page.end())
                (*itl)->error_page[it->first] = it->second;
        }
        if (!(*itl)->client_max_body_size)
            (*itl)->client_max_body_size = this->client_max_body_size;
        if (!(*itl)->autoindex)
            (*itl)->autoindex = this->autoindex;
        if ((*itl)->index.empty())
            (*itl)->index = this->index;
    }
    /*for (int i = 0; i <= location_size; i++)
    {
        this->location[i]->listen = this->listen;
        this->location[i]->server_name = this->server_name;
        this->location[i]->accept_method = this->accept_method;
        this->location[i]->redirection = this->redirection;
        this->location[i]->cgi = this->cgi;
        if (this->location[i]->root.empty())
            this->location[i]->root = this->root;
        for (std::map<int, std::string>::iterator it = this->error_page.begin(); it != this->error_page.end(); ++it)
        {
            if (this->location[i]->error_page.find(it->first) == this->location[i]->error_page.end())
                this->location[i]->error_page[it->first] = it->second;
        }
        if (!this->location[i]->client_max_body_size)
            this->location[i]->client_max_body_size = this->client_max_body_size;
        if (!this->location[i]->autoindex)
            this->location[i]->autoindex = this->autoindex;
        if (this->location[i]->index.empty())
            this->location[i]->index = this->index;
    }*/
}

void    Server::setLocation(std::vector<std::string> &words)
{
    if (words[0] == "error_page")
        this->location.back()->setErrorPage(words);
    else if (words[0] == "index")
        this->location.back()->setIndex(words[1]);
    else if (words[0] == "client_max_body_size")
        this->location.back()->setBodySize(words);
    else if (words[0] == "root")
        this->location.back()->setRoot(words[1]);
    else if (words[0] == "autoindex")
        this->location.back()->setAutoindex(words[1]);
    else if (words[0] == "cgi")
        this->location.back()->setCGI(words[1]);
    else
        throw std::runtime_error("error_setLocation");
    /*location_size = pos;
    if (words[0] == "error_page")
        this->location[location_size]->setErrorPage(words);
    else if (words[0] == "index")
        this->location[location_size]->setIndex(words[1]);
    else if (words[0] == "client_max_body_size")
        this->location[location_size]->setBodySize(words);
    else if (words[0] == "root")
        this->location[location_size]->setRoot(words[1]);
    else if (words[0] == "autoindex")
        this->location[location_size]->setAutoindex(words[1]);
    else if (words[0] == "cgi")
        this->location[location_size]->setCGI(words[1]);
    else
        throw std::runtime_error("error_setLocation");*/
}

void    Server::make_location()
{
    this->location.push_back(new Location());
}

void Server::printValuesServer() const {
            std::cout << "\033[31m";
            std::cout << "Server Name: " << server_name << std::endl;
            std::cout << "\033[0m"; 
            std::cout << "Accept Method: " << accept_method << std::endl;
            std::cout << "Client Max Body Size: " << client_max_body_size << std::endl;
            std::cout << "Redirection: " << redirection << std::endl;
            std::cout << "Root: " << root << std::endl;
            std::cout << "Autoindex: " << autoindex << std::endl;
            std::cout << "Index: " << index << std::endl;
            std::cout << "CGI: " << cgi << std::endl;

            // Imprimir listen (direcciones)
            std::cout << "Listen Addresses: " << std::endl;
            for (size_t i = 0; i < listen.size(); i++) {
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(listen[i].sin_addr), ip, INET_ADDRSTRLEN);
                std::cout << "  IP: " << ip << ", Port: " << ntohs(listen[i].sin_port) << std::endl;
            }

            // Imprimir error_page (páginas de error)
            std::cout << "Error Pages: " << std::endl;
            for (std::map<int, std::string>::const_iterator it = error_page.begin(); it != error_page.end(); ++it) {
                std::cout << "  Error Code: " << it->first << ", Page: " << it->second << std::endl;
            }

            // Imprimir cada Location en el vector location
            std::cout << "Locations (" << location.size() << "): " << std::endl;
            for (size_t i = 0; i < location.size(); ++i) {
                std::cout << "\033[33m"; 
                std::cout << "Location " << i + 1 << ":" << std::endl;
                std::cout << "\033[0m"; 
                location[i]->printValues();
            }
        }