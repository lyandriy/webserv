# pragma once
#ifndef CGI_HPP
#define CGI_HPP
#include "Webserver.hpp"

class CGI
{
    private:
    //variables de response
        std::string root;
        std::string uri;//must be either a binary executable, or a script
        std::map<std::string, std::string> params;
        std::map<std::string, std::string>  headers;
        std::vector<char>	body;
        std::string accept_method;

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
        void    setAcceptMethod(std::string);

        std::string getRoot() const;
        std::string getURI() const;
        std::map<std::string, std::string>  getParams() const;
        std::vector<char>   getBody() const;
        int getFDread() const;
        int getFDwrite() const;
        pid_t   getPid() const;
        std::string getAcceptMethod() const;

        int     makeProcess();
        void    make_execve();
        void    deleteArray();
        int     control_fd(int &);
        void    dupEnv(std::map<std::string, std::string> &);
};

#endif