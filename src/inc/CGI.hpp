# pragma once
#ifndef CGI_HPP
#define CGI_HPP
#include "Webserver.hpp"

class CGI
{
    private:
    //variables de reesponse
        std::string root;
        std::string uri;//must be either a binary executable, or a script
        std::map<std::string, std::string> params;
        std::vector<char>	body;

        //variables de cgi
        int fd_pipe[2];
        pid_t pid;
        char    **envp;
        char    **argv;

    public:
        CGI();
        ~CGI();
        CGI(const CGI &other);
        CGI &operator=(const CGI &);
        CGI(const Response &);

        void    setRoot(std::string);
        void    setURI(std::string);
        void    setParams(std::map<std::string, std::string>);
        void    setBody(std::vector<char>);

        std::string getRoot() const;
        std::string getURI() const;
        std::map<std::string, std::string>  getParams() const;
        std::vector<char>   getBody() const;
        int getFd() const;
        pid_t   getPid() const;

        int    makeProcess();
        void    make_execve();
        void    deleteArray();
        void printArgumentsAndEnvironment();
};

#endif