# include "../inc/Webserver.hpp"

Location::Location(){
    accept_method.get = -1;
    accept_method.post = -1;
    accept_method.del = -1;
    cgi = -1;
    autoindex = -1;
    client_max_body_size = 0;
}

Location::~Location(){}

Location::Location(const Location &other)
{
    *this = other;
}

Location &Location::operator=(const Location &other)
{
    this->listen = other.listen;
    this->server_name = other.server_name;
    this->root = other.root;
    this->index = other.index;
    this->accept_method = other.accept_method;
    this->redirection = other.redirection;
    this->error_page = other.error_page;
    this->client_max_body_size = other.client_max_body_size;
    this->autoindex = other.autoindex;
    this->cgi = other.cgi;
    this->locationUri = other.locationUri;
    return *this;
}

void    Location::setListen(std::string word)
{
    int port;
    struct sockaddr_in socket_addr;

    port = atoi(word.c_str());
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = INADDR_ANY;
    listen.push_back(socket_addr);
}

void    Location::setServerName(std::string server_name)
{
    this->server_name = server_name; 
}

void    Location::setRoot(std::string root)
{
    this->root = root;
}

void    Location::setIndex(std::string index)
{
    this->index = index;
}

void    Location::setAcceptMethod(std::vector<std::string> &words)
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

void    Location::setRedirection(std::string words)
{
    redirection = words;
}

void    Location::setErrorPage(std::vector<std::string> &words)
{
    std::string str_number;
    int int_number;

    for (size_t a = 1; a < (words.size() - 1); a++)
    {
        str_number.clear();
        for (size_t i = 0; (i < words[a].size() && isdigit(words[a][i])); i++)
            str_number += words[a][i];
        int_number = atoi(str_number.c_str());
        this->error_page[int_number] = words[words.size() - 1];
    }
}

void    Location::setBodySize(std::vector<std::string> &words)
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

void    Location::setAutoindex(std::string autoindex)
{
    if (autoindex == "on")
        this->autoindex = 1;
    else if (autoindex == "off")
        this->autoindex = 0;
}

void    Location::setCGI(std::string cgi)
{
    if (cgi == "on")
        this->cgi = 1;
    else if (cgi == "off")
        this->cgi = 0;
}

void    Location::setUri(std::string Uri)
{
    this->locationUri = Uri;
}

std::vector<struct sockaddr_in>    Location::getListen() const
{
    return(this->listen);
}

std::string    Location::getServerName() const
{
    return(this->server_name);
}

std::string    Location::getRoot() const
{
    return(this->root);
}

std::string    Location::getIndex() const
{
    return(this->index);
}

httpMethods    Location::getAcceptMethod() const
{
    return(this->accept_method);
}

std::string     Location::getRedirection() const
{
    return(this->redirection);
}

std::map<int, std::string>    Location::getErrorPage() const
{
    return(this->error_page);
}

int    Location::getBodySize() const
{
    return(this->client_max_body_size);
}

int    Location::getAutoindex() const
{
    return(this->autoindex);
}

int    Location::getCGI() const
{
    return(this->cgi);
}

std::string Location::getUri() const{
    return (this->locationUri);
}

Location    Location::clone() const
{
    Location	clon = Location(*this);
	return (clon);
}
