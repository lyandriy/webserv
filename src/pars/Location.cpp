#include "Location.hpp"

#include <cstdlib>

Location::Location(){}

Location::~Location(){}

Location::Location(const Location &other){}

Location &Location::operator=(const Location &other){return *this;}

void    Location::setErrorPage(std::vector<std::string> &words)
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
    /*for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }*/
}

void    Location::setBodySize(std::vector<std::string> &words)
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

void    Location::setRoot(std::string root)
{
    this->root = root;
}

void    Location::setAutoindex(std::string autoindex)
{
    this->autoindex = autoindex;
}

void    Location::setIndex(std::string index)
{
    this->index = index;
}

/*void    Location::setCGI(std::string cgi)
{
    this->cgi = cgi;
}*/

std::string    Location::getServerName() const
{
    return(this->server_name);
}

std::vector<struct sockaddr_in>    Location::getListen() const
{
    return(this->listen);
}

std::string    Location::getAcceptMethod() const
{
    return(this->accept_method);
}

std::map<int, std::string>    Location::getErrorPage() const
{
    return(this->error_page);
}

unsigned long long int    Location::getBodySize() const
{
    return(this->client_max_body_size);
}

std::string    Location::getRedirection() const
{
    return(this->redirection);
}

std::string    Location::getRoot() const
{
    return(this->root);
}

std::string    Location::getAutoindex() const
{
    return(this->autoindex);
}

std::string    Location::getIndex() const
{
    return(this->index);
}

/*std::string    Location::getCGI() const
{
    return(this->cgi);
}*/

/*Location::Location(Server &other){
    this->listen = other.getListen();
    this->server_name = other.getServerName();
    this->accept_method = other.getAcceptMethod();
    this->error_page = other.getErrorPage();
    this->client_max_body_size = other.getBodySize();
    this->redirection = other.getRedirection();
    this->root = other.getRoot();
    this->autoindex = other.getAutoindex();
    this->index = other.getIndex();
    //this->cgi = other->getCGI();
}*/

void Location::printValues() const {
    std::cout << "Server Name: " << server_name << std::endl;
    std::cout << "Accept Method: " << accept_method << std::endl;
    std::cout << "Client Max Body Size: " << client_max_body_size << std::endl;
    std::cout << "Redirection: " << redirection << std::endl;
    std::cout << "Root: " << root << std::endl;
    std::cout << "Autoindex: " << autoindex << std::endl;
    std::cout << "Index: " << index << std::endl;
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
}