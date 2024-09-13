#include "Parser.hpp"

Parser::Parser(){
    this->server_size = 0;
    this->location_size = -1;
    inServerBlock = false;
}

Parser::~Parser(){
    if (in_file.is_open())
        in_file.close();
}

Parser::Parser(const std::string file) : in_file(file.c_str())
{
    inServerBlock = false;
    this->server_size = 0;
    this->location_size = -1;
    if (!in_file.is_open())
        throw std::runtime_error("Error: Invalid file.");
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
            throw std::runtime_error("invalid number of arguments in listen.");
    }
    if (!str_number.empty())
    {
        int_number = atoi(str_number.c_str());
        str_number.clear();
        if (int_number > 65535)
            throw std::runtime_error("invalid port in " + words[1] + " of the listen directive.");
    }
    return (1);
}

int    Parser::server_name(){
    for (int i = 0; i < words[1].size(); i++)
    {
        if (!std::isalnum(words[1][i]) && words[1][i] != '.' && words[1][i] != '-')
            throw std::runtime_error("server name is invalid.");
    }
    return(1);
}

int    Parser::root(){

    if (valid_path(words[1]))
        return (1);
    throw std::runtime_error("root is invalid.");
}

int    Parser::redirection(){

    if (words[1] == "301" || words[1] == "302")
    {
            if (valid_path(words[2]))
                return (1);
    }
    throw std::runtime_error("redirection is invalid.");
}

bool    Parser::valid_path(std::string word)
{
    for (int i = 0; i < word.size(); i++)
    {
        if (!(std::isalnum(word[i]) || word[i] == '/' 
            || word[i] == '-' || word[i] == '_' || word[i] == '.'))
            return (false);
    }
    return (true);
}

int    Parser::index(){
    for (int i = 0; i < words[1].size(); i++)
    {
        if (!isprint(words[1][i]) || words[1][i] == '/' || words[1][i] == '*' || words[1][i] == '$')
            throw std::runtime_error("index is invalid.");
    }
    return (1);
}

int    Parser::accept_method(){
    for (int i = 1; i < words.size(); i++)
    {
        if (words[i] != "GET" && words[i] != "POST" && words[i] != "DELETE")
            throw std::runtime_error("accept method is invalid.");
    }
    return (1);
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
            if (!error_code(words[a]))
                throw std::runtime_error("error page is invalid.");
        }
        if (a == (words.size() - 1))
        {
            for (int i = 0; i < words[a].size(); i++)
            {
                if (!isprint(words[a][i]) || words[a][i] == '*' || words[a][i] == '$')
                    throw std::runtime_error("error page is invalid.");
            }
            return (1);
        }
    }
    throw std::runtime_error("error page is invalid.");
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
    throw std::runtime_error("client_max_body_size is invalid.");
}

int    Parser::autoindex(){
    if (words[1] == "off" || words[1] == "on")
        return (1);
    throw std::runtime_error("autoindex is invalid.");
}

int    Parser::cgi(){
    if (words[1] == "off" || words[1] == "on")
        return (1);
    throw std::runtime_error("cgi is invalid.");
}

void    Parser::key_words_location()
{
    if (words[0] == "error_page" && error_page() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "index" && words.size() == 2 && index() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "client_max_body_size" && words.size() == 2 && client_max_body_size() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "root" && words.size() == 2 && root() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "autoindex" && words.size() == 2 && autoindex() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "cgi" && words.size() == 2 && cgi() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "redirection" && words.size() == 3 && redirection() == 1)
        this->server[server_size].setLocation(words);
    else if (words[0] == "accept_method" && accept_method() == 1)
        this->server[server_size].setLocation(words);
    else
        throw std::runtime_error("Error: Invalid keyword " + words[0] + "!");
}

void    Parser::location_pars()
{
    std::vector<std::string>    location_words;

    if (location_size == -1)
        location_size = 0;
    this->server[server_size].make_location();
    this->server[server_size].setUri(words[1], location_size);
    words.erase(words.begin(), words.begin()+3);
    (words.end() - 1)->push_back(';');
    std::getline(in_file, line, '}');
    split(line);
    location_words = words;
    words.clear();
    for (std::vector<std::string>::iterator it = location_words.begin(); it != location_words.end(); ++it)
    {
        if ((*it)[(*it).size() - 1] != ';')
            words.push_back(*it);
        else
        {
            (*it).erase((*it).size() - 1);
            words.push_back(*it);
            key_words_location();
            words.clear();
        }
    }
    location_size++;
}

void    Parser::check_content()
{
    if (this->server[server_size].getServerName().empty())
        throw std::runtime_error("error. not server name");
    if (this->server[server_size].getListen().empty())
        this->server[server_size].setListen("80");  
}

void    Parser::key_words_server()
{
    if (words[0] == "listen" && words.size() == 2 && listen() == 1)
        this->server[server_size].setListen(words[1]);
    else if (words[0] == "server_name" && words.size() == 2 && server_name() == 1)
        this->server[server_size].setServerName(words[1]);
    else if (words[0] == "accept_method" && accept_method() == 1)
        this->server[server_size].setAcceptMethod(words);
    else if (words[0] == "error_page" && error_page() == 1)
        this->server[server_size].setErrorPage(words);
    else if (words[0] == "index" && words.size() == 2 && index() == 1)
        this->server[server_size].setIndex(words[1]);
    else if (words[0] == "client_max_body_size" && words.size() == 2 && client_max_body_size() == 1)
        this->server[server_size].setBodySize(words);
    else if (words[0] == "root" && words.size() == 2 && root() == 1)
        this->server[server_size].setRoot(words[1]);
    else if (words[0] == "autoindex" && words.size() == 2 && autoindex() == 1)
        this->server[server_size].setAutoindex(words[1]);
    else if (words[0] == "cgi" && words.size() == 2 && cgi() == 1)
        this->server[server_size].setCGI(words[1]);
    else if (words[0] == "redirection" && words.size() == 3 && redirection() == 1)
        this->server[server_size].setRedirection(words);
    else if (words.size() > 4 && words[0] == "location" && words[1][0] == '/' && words[2] == "{")
        location_pars();
    else
        throw std::runtime_error("*Error: Invalid keyword " + words[0] + ".");
}

void    Parser::server_pars()
{
    if (!words.empty())
    {
        key_words_server();
        words.clear();
    }
    while (std::getline(in_file, line, ';'))
    {
        if (!line.empty())
        {
            split(line);
            if ((words[0] == "}" || words[0][0] == '}') && inServerBlock == true)
            {
                check_content();
                inServerBlock = false;
                if (location_size != -1)
                    this->server[server_size].fillLocation();
                server_size++;
                if (words[0] == "}")
                    words.erase(words.begin());
                else
                    words[0].erase(0, 1);
                return ;
            }
            else
                key_words_server();
            words.clear();
        }
        else
            std::runtime_error("Error file.");
    }
}

void    Parser::IaMServer()
{
    this->server.push_back(Server());
    inServerBlock = true;
    if (words.size() == 1 && words[0] == "server")
        words.clear();
    server_pars();
    if (!words.empty() && words[0] == "server")
    {
        location_size = -1;
        words.erase(words.begin(), words.begin()+2);
        IaMServer();
    }
    else if (!words.empty())
        throw std::runtime_error("Error: Unrecognized keyword " + words[0] + ".");
}

std::vector<Server> Parser::conf_file()
{
    while (std::getline(in_file, line, '{'))
    {
        if (!line.empty())
        {
            split(line);
            if (words[0] == "server" && words.size() == 1)
                IaMServer();
            else
            {
                words.clear();
                throw std::runtime_error("Error: Unrecognized keyword " + words[0] + ".");
            }
        }
        else
            throw std::runtime_error("Error file.");
    }
    for (int i = 0; i < server_size; i++)
        this->server[i].printValuesServer();
    return (server);
}
