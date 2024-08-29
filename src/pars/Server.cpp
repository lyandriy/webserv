#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

Server::Server(const Server &other){}

Server &Server::operator=(const Server &other){}

void    Server::setListen(std::string listen)
{
    this->listen = listen;
}

void    Server::setServerName(std::string server_name)
{
    this->server_name = server_name; 
}

void    Server::setAcceptMethod(std::string accept_method)
{
    this->accept_method = accept_method;
}

void    Server::setErrorPage(std::string error_page)
{
    this->error_page = error_page;
}

void    Server::setBodySize(std::string client_max_body_size)
{
    this->client_max_body_size = client_max_body_size;
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

void    Server::setCGI(std::string cgi)
{
    this->cgi = cgi;
}

std::string    Server::getServerName() const
{
    return(this->server_name);
}

std::string    Server::getListen() const
{
    return(this->listen);
}

std::string    Server::getAcceptMethod() const
{
    return(this->accept_method);
}

std::string    Server::getErrorPage() const
{
    return(this->error_page);
}

std::string    Server::getBodySize() const
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

std::string    Server::getCGI() const
{
    return(this->cgi);
}

void    Server::createLocation()
{
    this->location.push_back(new Location(this));
}
void    Server::setLocation(std::vector<std::string> &words)
{
    Location*   back = location.back();
    if (words[0] == "error_page")
        back->setErrorPage(words[1]);
    else if (words[0] == "index")
        back->setIndex(words[1]);
    else if (words[0] == "client_max_body_size")
        back->setBodySize(words[1]);
    else if (words[0] == "root")
        back->setRoot(words[1]);
    else if (words[0] == "autoindex")
        back->setAutoindex(words[1]);
    else if (words[0] == "cgi")
        back->setCGI(words[1]);
    else
        throw std::runtime_error("error");
}
