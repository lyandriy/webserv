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
        int fd_file = -1;
        int fd_pipe[2];
        pid_t pid;
        char    **envp;//should contain the filename  associated  with the  file  being  executed. The argv array must be terminated by a NULL pointer.
        char    **argv;//conventionally of the form key=value, which are  passed  as the environment of the new program.  The envp array must be terminated by a NULL pointer.

        CGI();
    public:
        ~CGI();
        CGI &operator=(const CGI &);
        CGI(const Response &);

        int make_cgi();
        void    make_execve();
};

#endif