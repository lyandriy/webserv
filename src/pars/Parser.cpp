#include "Parser.hpp"

Parser::Parser(){
    this->server_size = 0;
    this->location_size = 0;
}

Parser::~Parser(){
    if (in_file.is_open())
        in_file.close();
}

Parser::Parser(const std::string file) : in_file(file.c_str())
{
    if (!in_file.is_open())
        throw std::runtime_error("error");
}

void    Parser::split(std::string &line, std::vector<std::string> &words)
{
    std::istringstream  iss;
    std::string         line;

    iss.str(line);
    while (iss >> line)
    {
        std::cout << line << std::endl;
        words.push_back(line);
    }
    iss.clear();
}

int    Parser::listen(){
    
}

int    Parser::server_name(){

}

int    Parser::accept_method(){

}

int    Parser::error_page(){

}

int    Parser::index(){

}

int    Parser::client_max_body_size(){

}

int    Parser::redirection(){

}

int    Parser::root(){

}

int    Parser::autoindex(){

}

int    Parser::cgi(){

}

void    Parser::key_words_server()
{
    if (words[0] == "listen" && listen() == 1)
        this->server[server_size]->setListen(words[1]);
    else if (words[0] == "server_name" && server_name() == 1)
        this->server[server_size]->setServerName(words[1]);
    else if (words[0] == "accept_method" && accept_method() == 1)
        this->server[server_size]->setAcceptMethod(words[1]);
    else if (words[0] == "error_page" && error_page() == 1)
        this->server[server_size]->setErrorPage(words[1]);
    else if (words[0] == "index" && index() == 1)
        this->server[server_size]->setIndex(words[1]);
    else if (words[0] == "client_max_body_size" && client_max_body_size() == 1)
        this->server[server_size]->setBodySize(words[1]);
    else if (words[0] == "root" && root() == 1)
        this->server[server_size]->setRoot(words[1]);
    else if (words[0] == "autoindex" && autoindex() == 1)
        this->server[server_size]->setAutoindex(words[1]);
    else if (words[0] == "cgi" && cgi() == 1)
        this->server[server_size]->setCGI(words[1]);
    else if (words[0] == "redirection" && redirection() == 1)
        this->server[server_size]->setRedirection(words[1]);
    else
        throw std::runtime_error("error");
}

void    Parser::key_words_location()
{
    if (words[0] == "error_page" && error_page() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "index" && index() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "client_max_body_size" && client_max_body_size() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "root" && root() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "autoindex" && autoindex() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "cgi" && cgi() == 1)
        this->server[server_size]->setLocation(words);
    else
        throw std::runtime_error("error");
}

void    Parser::location_key()
{
    if ((words[1] == "/" || words[1][0] == '/') && words[2] == "{")
    {
        words.erase(words.begin(), words.begin()+3);//elimina location, /, y {
        key_words_location();
    }
    else if (words[1] == "=" && words[3] == "{")//si url tiene que ser igual
    {
        words.erase(words.begin(), words.begin()+4);//elimina location, =, url, y {
        key_words_location();
    }
    else
        throw std::runtime_error("error");
    location_size++;
}

void    Parser::server_pars()
{
    std::string		            line;

    this->server.push_back(new Server());//crear un objeto nuevo de server
    while (std::getline(in_file, line, ';'))//lee hasta encontrar ;
    {
        if (!line.empty())//si encuentra ;
        {
            split(line, words);//separa la linea en palabras
            if (words[0] == "location")//cuando encuentra location
                location_key();
            else
            {
                words.erase(words.begin());
                key_words_server();//cuando no es location
            }
            words.clear();
        }
        else
            throw std::runtime_error("error");
    }
    server_size++;
}

void Parser::conf_file()
{
    std::string		            line;

    while (std::getline(in_file, line, '{'))
    {
        if (!line.empty())//lee haste ell final de fd
        {
            split(line, words);//separa la linea en palabras
            if (words[0] == "server" && words.size() == 1)//si encuentra server en primera posicion y no ha nada mas despues de server
            {
                words.clear();
                server_pars();
            }
            else
            {
                words.clear();
                throw std::runtime_error("error");
            }
        }
        else
            throw std::runtime_error("error");
    }
}