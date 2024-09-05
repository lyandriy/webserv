#include "Parser.hpp"

Parser::Parser(){
    this->server_size = 0;
    this->location_size = 0;
    inLocationBlock = false;
    inServerBlock = false;
}

Parser::~Parser(){
    if (in_file.is_open())
        in_file.close();
}

Parser::Parser(const std::string file) : in_file(file.c_str())
{
    inServerBlock = false;
    inLocationBlock = false;
    this->server_size = 0;
    this->location_size = 0;
    if (!in_file.is_open())
        throw std::runtime_error("error_Parder_constructor");
}

void    Parser::split(std::string &line)
{
    std::istringstream  iss(line);
    std::string         line_;

    while (iss >> line_)
        words.push_back(line_);
    iss.clear();
}
int    Parser::listen(){
    std::string str_number;
    int int_number;

    for (int i = 0; words[1][i]; i++)
    {
        if (isdigit(words[1][i]))
            str_number += words[1][i];
        else
            return (0);
    }
    if (!str_number.empty())
    {
        int_number = atoi(str_number.c_str());
        str_number.clear();
        if (int_number > 65535)
            return (0);
    }
    return (1);
}

int    Parser::server_name(){
    for (int i = 0; i < words[1].size(); i++)
    {
        if (!std::isalnum(words[1][i]) && words[1][i] != '.' && words[1][i] != '-')
            return (0);
    }
    return(1);
}

int    Parser::accept_method(){
    if (words[1] == "GET" || words[1] == "POST")
        return (1);
    return (0);
}

int Parser::error_code(std::string str_number)
{
    int int_number;
    int code[38] = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412,
                  413, 414, 415, 416, 417, 421, 422, 423, 424, 425, 426, 428, 429,
                  431, 451, 500, 501, 502, 503, 504, 505, 506, 507, 508, 511};

    if (!str_number.empty())
       int_number = atoi(str_number.c_str());
    for (int i = 0; i < 38; i++)
    {
        if (int_number == code[i])
            return (1);
    }
    return (0);
}

int Parser::is_number(std::string word)
{
    std::string str_number;

    for (int i = 0; i < word.size(); i++)
    {
        if (isdigit(word[i]))
            str_number += word[i];
        else
        {
            str_number.clear();
            return (0);
        }
    }
    return (1);
}

int    Parser::error_page(){
    int a = 1;
    
    
    if (words.size() >= 3)
    {
        for (a = 1; (a < words.size() && is_number(words[a])); a++)
        {
            std::cout << words[a] << std::endl;
            if (!error_code(words[a]))
            {
                std::cout << words[a] << std::endl;
                return (0);
            }
        }
        /*for (int i = 0; i != 7; i++)
            std::cout << words[i] << "   " << a << " y " << words.size() << std::endl;*/
        if (a == (words.size() - 1))
        {
            for (int i = 0; i < words[a].size(); i++)
            {
                if (!isprint(words[a][i]) || words[a][i] == '*' || words[a][i] == '$')
                {
                    std::cout << words[a] << std::endl;
                    return (0);
                }
            }
            return (1);
        }
    }
    //std::cout << "donde estoy " << words[a] << std::endl;
    return (0);
}

int    Parser::index(){
    for (int i = 0; i < words[1].size(); i++)
    {
        if (!isprint(words[1][i]) || words[1][i] == '/' || words[1][i] == '*' || words[1][i] == '$')
            return (0);
    }
    return (1);
}

int    Parser::client_max_body_size(){
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
        if ((words[1][i] == 'M' || words[1][i] == 'k' || words[1][i] == 'G') && i == (words[1].size() - 1))
            return (1);
    }
    return (0);
}

int    Parser::redirection(){
    if (valid_path())
        return (1);
    return (0);
}

int    Parser::root(){

    if (valid_path())
        return (1);
    return (0);
}

int    Parser::autoindex(){
    if (words[1] == "off" || words[1] == "on")
        return (1);
    return (0);
}

/*int    Parser::cgi(){

}*/

bool    Parser::valid_path()
{
    for (int i = 0; i < words[1].size(); i++)
    {
        if (!(std::isalnum(words[1][i]) || words[1][i] == '/' 
            || words[1][i] == '-' || words[1][i] == '_' || words[1][i] == '.'))
            return (false);
    }
    return (true);
}

void    Parser::location_key()
{
    std::vector<std::string>    location_words;
    std::string line;
    this->server[server_size]->make_location();
    words.erase(words.begin(), words.begin()+3);
    (words.end() - 1)->push_back(';');
    std::getline(in_file, line, '}');
    split(line);
    location_words = words;
    words.clear();
    /*for (int i = 0; i < location_words.size(); i++)
                std::cout << location_words[i] << "   " << " y " << location_words.size() << std::endl;
    */
    for (std::vector<std::string>::iterator it = location_words.begin(); it != location_words.end(); ++it)
    {
        std::cout <<"aqui que pasa " << *it << std::endl;
        if ((*it)[(*it).size() - 1] != ';')
            words.push_back(*it);
        else
        {
            (*it).erase((*it).size() - 1);
            words.push_back(*it);
            key_words_location();
            words.clear();
            /*for (int i = 0; i < words.size(); i++)
                std::cout << words[i] << "   " << " y " << words.size() << std::endl;*/
        }
    }
    location_size++;
}

void    Parser::key_words_server()
{
    if (words[0] == "listen" && words.size() == 2 && listen() == 1)
        this->server[server_size]->setListen(words[1]);
    else if (words[0] == "server_name" && words.size() == 2 && server_name() == 1)
        this->server[server_size]->setServerName(words[1]);
    else if (words[0] == "accept_method" && words.size() == 2 && accept_method() == 1)
        this->server[server_size]->setAcceptMethod(words[1]);
    else if (words[0] == "error_page" && error_page() == 1)
        this->server[server_size]->setErrorPage(words);
    else if (words[0] == "index" && words.size() == 2 && index() == 1)
        this->server[server_size]->setIndex(words[1]);
    else if (words[0] == "client_max_body_size" && words.size() == 2 && client_max_body_size() == 1)
        this->server[server_size]->setBodySize(words);
    else if (words[0] == "root" && words.size() == 2 && root() == 1)
        this->server[server_size]->setRoot(words[1]);
    else if (words[0] == "autoindex" && words.size() == 2 && autoindex() == 1)
        this->server[server_size]->setAutoindex(words[1]);
    /*else if (words[0] == "cgi" && words.size() == 2 && cgi() == 1)
        this->server[server_size]->setCGI(words[1]);*/
    else if (words[0] == "redirection" && words.size() == 2 && redirection() == 1)
        this->server[server_size]->setRedirection(words[1]);
    else if (words.size() > 4 && words[0] == "location" && words[1][0] == '/' && words[2] == "{")
        location_key();
    else
        throw std::runtime_error("error_key_words_server");
}

void    Parser::key_words_location()
{
    std::cout << error_page() << " aqui " << words[0] << std::endl;
    if (words[0] == "error_page" && error_page() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "index" && words.size() == 2 && index() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "client_max_body_size" && words.size() == 2 && client_max_body_size() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "root" && words.size() == 2 && root() == 1)
        this->server[server_size]->setLocation(words);
    else if (words[0] == "autoindex" && words.size() == 2 && autoindex() == 1)
        this->server[server_size]->setLocation(words);
    /*else if (words[0] == "cgi" && words.size() == 2 && cgi() == 1)
        this->server[server_size]->setLocation(words);*/
    else
        throw std::runtime_error("error_key_words_location1");
}

void    Parser::server_pars()
{
    std::string line;

    while (std::getline(in_file, line, ';'))
    {
        if (!line.empty())
        {
            split(line);
            if (words[0] == "}" && inServerBlock == true)//falta hacer esta caso: server{...}server {...}
            {
                inServerBlock = false;
                this->server[server_size]->fillLocation();
                server_size++;
                return ;
            }
            else
                key_words_server();
            words.clear();
        }
        else
             std::runtime_error("error");
    }
}

void Parser::conf_file()
{
    std::string line;

    while (std::getline(in_file, line, '{'))
    {
        if (!line.empty())
        {
            split(line);
            if (words[0] == "server" && words.size() == 1)
            {
                this->server.push_back(new Server());
                inServerBlock = true;
                words.clear();
                server_pars();
            }
            else
            {
                words.clear();
                throw std::runtime_error("error_not_server");
            }
        }
        else
            throw std::runtime_error("error_conf_file");
    }
    std::cout << "server_size = " << server_size << std::endl;
    this->server[server_size - 1]->printValuesServer();
}