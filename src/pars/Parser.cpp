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
    this->server_size = 0;
    this->location_size = 0;
    if (!in_file.is_open())
        throw std::runtime_error("error_Parder_constructor");
}

void    Parser::split(std::string &line, std::vector<std::string> &words)
{
    std::istringstream  iss(line);
    std::string         line_;

    while (iss >> line_)
    {
        std::cout << line_ << std::endl;
        words.push_back(line_);
    }
    iss.clear();
    std::cout << "fin de split" << std::endl;
}

int    Parser::listen(){
    int octet = 0;
    int port =  0;
    std::string str_number;
    int int_number;

    for (int i = 0; words[1][i]; i++)
    {
        if (isdigit(words[1][i]))
            str_number += words[1][i];
        else if (words[1][i] == '.')
        {
            if (!str_number.empty())
            {
                int_number = atoi(str_number.c_str());
                str_number.clear();
                if (int_number > 256 || octet == 4)
                    return (0);
                octet++;
            }
            else
                return (0);
        }
        else if (words[1][i] == ':')
        {
            if (!str_number.empty())
            {
                int_number = atoi(str_number.c_str());
                str_number.clear();
                if (int_number > 65535 || port == 1)
                    return (0);
                port++;
            }
            else
                return (0);
        }
        else
            return (0);
    }
    if (!str_number.empty())
    {
        int_number = atoi(str_number.c_str());
        str_number.clear();
        if (int_number > 65535 || port == 1)
            return (0);
        port++;
    }
    if ((octet != 0 && octet != 4) || port != 1)
        return (0);
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

int    Parser::error_page(){
    int a = 1;

    for (a = 1; a < words.size(); a++)
    {
        for (int i = 0; i < words[a].size(); i++)
        {
            if (i == 3 || a < 63)
                return (0);
            else if (!isdigit(words[a][i]))
                break;
        }
    }
    if (a != 1 && a == (words.size() - 1))
    {
        for (int i = 0; i < words[a].size(); i++)
        {
            if (!isprint(words[a][i]) || words[a][i] == '/' || words[a][i] == '*' || words[a][i] == '$')
                return (0);
        }
        return (1);
    }
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
        if ((words[1][i] == 'M' || words[1][i] == 'k' || words[1][i] == 'G') && !words[0][i++])
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
    for (int i = 0; i < words[0].size(); i++)
    {
        if (!(std::isalnum(words[1][i]) || words[1][i] == '/' 
            || words[1][i] == '-' || words[1][i] == '_' || words[1][i] == '.'))
            return (false);
    }
    return (true);
}

void    Parser::key_words_server()
{
    std::cout << words[0] << " " << words[1] << std::endl;
    std::cout << words.size() << " " << listen() << std::endl;
    if (words[0] == "listen" && words.size() == 2 && listen() == 1)
    {
        std::cout << server_size << " entro" << std::endl;
        this->server[server_size]->setListen(words);
    }
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
    else
        throw std::runtime_error("error_key_words_server");
}

void    Parser::key_words_location()
{
    if (words[0] == "error_page" && words.size() == 2 && error_page() == 1)
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
        throw std::runtime_error("error_key_words_location");
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
        throw std::runtime_error("error_locationKey");
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
            {
                std::cout << "parser location" << std::endl;
                location_key();
            }
            else
            {
                std::cout << "directivas" << words[0] << std::endl;
                key_words_server();//cuando no es location
            }
            words.clear();
        }
        else
             std::runtime_error("error");
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
                std::cout << "parser server" << std::endl;
                words.clear();
                server_pars();
            }
            else
            {
                words.clear();
                throw std::runtime_error("error_conf_file1");
            }
        }
        else
            throw std::runtime_error("error_conf_file2");
    }
}
