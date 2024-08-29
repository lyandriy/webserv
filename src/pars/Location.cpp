#include "Location.hpp"

Location::Location(){}

Location::~Location(){}

Location::Location(const Location &other){}

Location &Location::operator=(const Location &other){}

void    Location::setErrorPage(std::string error_page)
{
    this->error_page = error_page;
}

void    Location::setBodySize(std::string client_max_body_size)
{
    this->client_max_body_size = client_max_body_size;
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

void    Location::setCGI(std::string cgi)
{
    this->cgi = cgi;
}

std::string    Location::getServerName() const
{
    return(this->server_name);
}

std::string    Location::getListen() const
{
    return(this->listen);
}

std::string    Location::getAcceptMethod() const
{
    return(this->accept_method);
}

std::string    Location::getErrorPage() const
{
    return(this->error_page);
}

std::string    Location::getBodySize() const
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

std::string    Location::getCGI() const
{
    return(this->cgi);
}

Location::Location(Server *other){
    this->listen = other->getListen();
    this->server_name = other->getServerName();
    this->accept_method = other->getAcceptMethod();
    this->error_page = other->getErrorPage();
    this->client_max_body_size = other->getBodySize();
    this->redirection = other->getRedirection();
    this->root = other->getRoot();
    this->autoindex = other->getAutoindex();
    this->index = other->getIndex();
    this->cgi = other->getCGI();
}