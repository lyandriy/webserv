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

    for (a = 1; a < (words.size() - 1); a++)
    {
        for (int i = 0; (isdigit(words[a][i]) && i < words[a].size()); i++)
            str_number += words[a][i];
        this->error_page[atoi(str_number.c_str())] = words[words.size() - 1];
    }
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
        if (words[0][i] == 'M' && !words[0][i++])
            int_number *= 1048576;
        else if (words[0][i] == 'k' && !words[0][i++])
            int_number *= 1024;
        else if (words[0][i] == 'G' && !words[0][i++])
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

Location::Location(Server &other){
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
}