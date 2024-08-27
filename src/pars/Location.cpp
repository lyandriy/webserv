#include "Location.hpp"

Location::Location(){}

Location::~Location(){}

Location::Location(const Location &other){}

Location &Location::operator=(const Location &other){}

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