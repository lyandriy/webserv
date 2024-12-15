# include "../inc/Webserver.hpp"

Server::Server(){
    accept_method.get = 1;
    accept_method.post = 1;
    accept_method.del = 1;
    client_max_body_size = 1048576;
    cgi = 0;
    autoindex = 0;
    index = "index.html";
}

Server::Server(int i){
   autoindex = i;
}

Server::~Server(){}

Server::Server(const Server &other)
{
    *this = other;
}

Server &Server::operator=(const Server &other)
{
    this->listen = other.listen;
    this->server_name = other.server_name;
    this->root = other.root;
    this->index = other.index;
    this->accept_method = other.accept_method;
    this->error_page = other.error_page;
    this->client_max_body_size = other.client_max_body_size;
    this->autoindex = other.autoindex;
    this->cgi = other.cgi;
    this->location = other.location;
    return *this;
}

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

void    Server::setRoot(std::string root)
{
    this->root = root;
}

void    Server::setIndex(std::string index)
{
    this->index = index;
}

void    Server::setAcceptMethod(std::vector<std::string> &words)
{
    accept_method.get = 1;
    accept_method.post = 1;
    accept_method.del = 1;
    if (std::find(words.begin(), words.end(), "GET") == words.end())
        accept_method.get = 0;
    if (std::find(words.begin(), words.end(), "POST") == words.end())
        accept_method.post = 0;
    if (std::find(words.begin(), words.end(), "DELETE") == words.end())
        accept_method.del = 0;
}

void    Server::setErrorPage(std::vector<std::string> &words)
{
    std::string str_number;
    int int_number;

    for (size_t a = 1; a < (words.size() - 1); a++)
    {
        for (size_t i = 0; (isdigit(words[a][i]) && i < words[a].size()); i++)
            str_number += words[a][i];
        int_number = atoi(str_number.c_str());
        str_number.clear();
        this->error_page[int_number] = words[words.size() - 1];
    }
}

void    Server::setBodySize(std::vector<std::string> &words)
{
    std::string str_number;
    int  int_number;
    size_t i = 0;

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

void    Server::setAutoindex(std::string autoindex)
{
    if (autoindex == "on")
        this->autoindex = 1;
    else if (autoindex == "off")
        this->autoindex = 0;
}

void    Server::setCGI(std::string cgi)
{
    if (cgi == "on")
        this->cgi = 1;
    else if (cgi == "off")
        this->cgi = 0;
}

void    Server::setUri(std::string Uri, int pos)
{
    this->location[pos].setUri(Uri);
}

void    Server::setLocation(std::vector<std::string> &words)
{
    if (words[0] == "error_page")
        this->location.back().setErrorPage(words);
    else if (words[0] == "index")
        this->location.back().setIndex(words[1]);
    else if (words[0] == "client_max_body_size")
        this->location.back().setBodySize(words);
    else if (words[0] == "root")
        this->location.back().setRoot(words[1]);
    else if (words[0] == "autoindex")
        this->location.back().setAutoindex(words[1]);
    else if (words[0] == "cgi")
        this->location.back().setCGI(words[1]);
    else if (words[0] == "redirection")
        this->location.back().setRedirection(words[1]);
    else if (words[0] == "accept_method")
        this->location.back().setAcceptMethod(words);
    else
        throw std::runtime_error("Error: Unrecognized keyword " + words[0]);
}

void    Server::fillLocation()
{
    for (std::vector<Location>::iterator itl = location.begin(); itl != location.end(); ++itl)
    {
        (*itl).listen = this->listen;
        (*itl).server_name = this->server_name;
        if ((*itl).accept_method.get == -1 && (*itl).accept_method.post == -1 && (*itl).accept_method.del == -1)
            (*itl).accept_method = this->accept_method;
        if ((*itl).cgi == -1)
            (*itl).cgi = this->cgi;
        if ((*itl).root.empty())
            (*itl).root = this->root;
        for (std::map<int, std::string>::iterator it = this->error_page.begin();
            it != this->error_page.end(); ++it)
        {
            if ((*itl).error_page.find(it->first) == (*itl).error_page.end())
                (*itl).error_page[it->first] = it->second;
        }
        if (!(*itl).client_max_body_size)
            (*itl).client_max_body_size = this->client_max_body_size;
        if ((*itl).autoindex == -1)
            (*itl).autoindex = this->autoindex;
        if ((*itl).index.empty())
            (*itl).index = this->index;
    }
}

void    Server::make_location()
{
    this->location.push_back(Location());
}

std::vector<struct sockaddr_in>    Server::getListen() const
{
    return(this->listen);
}

std::string    Server::getServerName() const
{
    return(this->server_name);
}

std::string    Server::getRoot() const
{
    return(this->root);
}

std::string    Server::getIndex() const
{
    return(this->index);
}

httpMethods    Server::getAcceptMethod() const
{
    return(this->accept_method);
}

std::map<int, std::string>    Server::getErrorPage() const
{
    return(this->error_page);
}

int    Server::getBodySize() const
{
    return(this->client_max_body_size);
}

int    Server::getAutoindex() const
{
    return(this->autoindex);
}

int   Server::getCGI() const
{
    return(this->cgi);
}

std::string    Server::getUri(int pos) const
{
    return (this->location[pos].getUri());
}

std::vector<Location>  Server::getLocation() const
{
    return (this->location);
}

void Server::printValuesServer() const {
            std::cout << "\033[31m";
            std::cout << "Server Name: " << server_name << std::endl;
            std::cout << "\033[0m"; 
            std::cout << "Accept Method: GET " << accept_method.get << " POST " << accept_method.post << " DELETE " << accept_method.del << std::endl;
            std::cout << "Client Max Body Size: " << client_max_body_size << std::endl;
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
                location[i].printValues();
            }
        }