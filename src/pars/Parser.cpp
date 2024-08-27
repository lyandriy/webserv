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

int    Parser::listen(std::vector<std::string> &words, int &pos){
    
}

int    Parser::server_name(std::vector<std::string> &words, int &pos){

}

int    Parser::accept_method(std::vector<std::string> &words, int &pos){

}

int    Parser::error_page(std::vector<std::string> &words, int &pos){

}

int    Parser::index(std::vector<std::string> &words, int &pos){

}

int    Parser::client_max_body_size(std::vector<std::string> &words, int &pos){

}

int    Parser::redirection(std::vector<std::string> &words, int &pos){

}

int    Parser::root(std::vector<std::string> &words, int &pos){

}

int    Parser::autoindex(std::vector<std::string> &words, int &pos){

}

int    Parser::cgi(std::vector<std::string> &words, int &pos){

}

void    Parser::key_words_server(std::vector<std::string> &words, int pos)
{
    if (words[pos] == "listen" && listen(words, pos) == 1)
        this->server[server_size]->setListen(words[pos]);
    else if (words[pos] == "server_name" && server_name(words, pos) == 1)
        this->server[server_size]->setServerName(words[pos]);
    else if (words[pos] == "accept_method" && accept_method(words, pos) == 1)
        this->server[server_size]->setAcceptMethod(words[pos]);
    else if (words[pos] == "error_page" && error_page(words, pos) == 1)
        this->server[server_size]->setErrorPage(words[pos]);
    else if (words[pos] == "index" && index(words, pos) == 1)
        this->server[server_size]->setIndex(words[pos]);
    else if (words[pos] == "client_max_body_size" && client_max_body_size(words, pos) == 1)
        this->server[server_size]->setBodySize(words[pos]);
    else if (words[pos] == "root" && root(words, pos) == 1)
        this->server[server_size]->setRoot(words[pos]);
    else if (words[pos] == "autoindex" && autoindex(words, pos) == 1)
        this->server[server_size]->setAutoindex(words[pos]);
    else if (words[pos] == "cgi" && cgi(words, pos) == 1)
        this->server[server_size]->setCGI(words[pos]);
    else if (words[pos] == "redirection" && redirection(words, pos) == 1)
        this->server[server_size]->setRedirection(words[pos]);
    else
        throw std::runtime_error("error");
}

void    Parser::key_words_location(std::vector<std::string> &words, int pos)
{
    if (words[pos] == "error_page" && error_page(words, pos) == 1)
        this->server[server_size]->setErrorPage(words[pos]);
    else if (words[pos] == "index" && index(words, pos) == 1)
        this->server[server_size]->setIndex(words[pos]);
    else if (words[pos] == "client_max_body_size" && client_max_body_size(words, pos) == 1)
        this->server[server_size]->setBodySize(words[pos]);
    else if (words[pos] == "root" && root(words, pos) == 1)
        this->server[server_size]->setRoot(words[pos]);
    else if (words[pos] == "autoindex" && autoindex(words, pos) == 1)
        this->server[server_size]->setAutoindex(words[pos]);
    else if (words[pos] == "cgi" && cgi(words, pos) == 1)
        this->server[server_size]->setCGI(words[pos]);
    else
        throw std::runtime_error("error");
}

void    Parser::location_key(std::vector<std::string> &words)
{
    this->location.push_back(new Location(server[server_size]));
    if (words[1] == "/" && words[2] == "{")
    {
        key_words_location(words, 3);
    }
    else if (words[1] == "=" && words[3] == "{")
    {

        key_words_location(words, 4);
    }
    else
        throw std::runtime_error("error");
    location_size++;
}

void    Parser::server_pars()
{
    std::string		            line;
    std::vector<std::string>    words;

    this->server.push_back(new Server());
    while (std::getline(in_file, line, ';'))
    {
        if (!line.empty())
        {
            split(line, words);
            if (words[0] == "location")
                location_key(words);
            else
                key_words_server(words, 0);
        }
        else
            throw std::runtime_error("error");
    }
    server_size++;
}

void Parser::conf_file()
{
    std::string		            line;
    std::vector<std::string>    words;

    while (std::getline(in_file, line, '{'))
    {
        if (!line.empty())
        {
            split(line, words);
            if (words[0] == "server" && words.size() == 1)
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