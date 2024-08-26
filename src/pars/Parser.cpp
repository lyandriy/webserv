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

void    Parser::key_words(std::vector<std::string> &words)
{
    for (int i = 0; i < 10; i++)
    {
        if (words[0] == key[i])
        {
            methods[i];
        }
    }
}

void    Parser::location_key(std::vector<std>>string> &words)
{
    server[server_size].location.bush_back(new Location());
    if (words[1] == '/')
    {}
    else if (words[1] == '=')
    {}
    if (words[1] == '{' || words[2] == '{')
    {
        key_words(words);
    }
    location_size++;
}

void    Parser::server_pars()
{
    std::string		            line;
    std::vector<std::string>    words;

    server.push_back(new Server()); 
    while (std::getline(in_file, line, ';'))
    {
        if (!line.empty())
        {
            split(line, words);
            if (words[0] == "location")
            {
                location_key(words);
            }
            else
            {
                key_words(words);
            }
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