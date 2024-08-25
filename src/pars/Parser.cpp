#include "Parser.hpp"

Parser::Parser(){}

Parser::~Parser(){
    if (in_file.is_open())
        in_file.close();
}

Parser::Parser(const std::string file) : in_file(file.c_str()), server(nullptr)
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

void    Parser::key_words(std::vector<std::string> &words)
{
    std::string    key[10] = {"listen", "server_name", "accept_method", "error_page", "index",
                            "client_max_body_size", "redirection", "root", "autoindex", "cgi"};
    void (Parser::*methods[])() = {
        &Parser::listen,
        &Parser::server_name,
        &Parser::accept_method,
        &Parser::error_page,
        &Parser::index,
        &Parser::client_max_body_size,
        &Parser::redirection,
        &Parser::root,
        &Parser::autoindex,
        &Parser::cgi
        };
    for (int i = 0; i < 10; i++)
    {
        if (words[0] == key[i])
        {
            methods[i];
        }
    }
}

void    Parser::server_pars()
{
    std::string		            line;
    std::vector<std::string>    words;

    while (std::getline(in_file, line, ';'))
    {
        if (!line.empty())
        {
            split(line, words);
            if (words[0] == "location")
            {
                
            }
            else
            {
                key_words(words);
            }
        }
        else
            throw std::runtime_error("error");
    }
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