# include "../inc/Webserver.hpp"

CGI::CGI(){}

CGI::~CGI(){}

CGI::CGI(const CGI &other)
{
    *this = other;
}

CGI &CGI::operator=(const CGI &other)
{
    this->root = other.root;
    this->uri = other.uri;
    this->params = other.params;
    this->headers = other.headers;
    this->body = other.body;
    this->accept_method = other.accept_method;
    this->fd_pipe[0] = other.fd_pipe[0];
    this->fd_pipe[1] = other.fd_pipe[1];
    this->pid = other.pid;
    this->envp = other.envp;
    this->argv = other.argv;
    return (*this);
}

CGI::CGI(const Response &response)
{
    this->root = response.getRoot();
    this->uri = response.getURI();
    this->params = response.getParams();
    this->headers = response.getHeaders();
    this->body = response.getBody();
    this->accept_method = response.getAcceptMethod();
    this->pid = -2;
    std::map<std::string, std::string>::iterator it;
}

void    CGI::setRoot(std::string root)
{
    this->root = root;
}

void    CGI::setURI(std::string uri)
{
    this->uri = uri;
}

void    CGI::setParams(std::map<std::string, std::string> params)
{
    this->params = params;
}

void    CGI::setBody(std::vector<char> body)
{
    this->body = body;
}

void    CGI::setAcceptMethod(std::string accept_method)
{
    this->accept_method = accept_method;
}

std::string CGI::getRoot() const
{
        return (this->root);
}

std::string CGI::getURI() const
{
    return (this->uri);
}

std::map<std::string, std::string>  CGI::getParams() const
{
    return (this->params);
}

std::vector<char>   CGI::getBody() const
{
    return (this->body);
}

int CGI::getFDread() const
{
    return (this->fd_pipe[0]);
}

int CGI::getFDwrite() const
{
    return (this->fd_pipe[1]);
}

pid_t   CGI::getPid() const
{
    return (this->pid);
}

std::string CGI::getAcceptMethod() const
{
    return (this->accept_method);
}

void    CGI::deleteArray()
{
    if (accept_method == "POST")
    {
        for (size_t i = 0; i != headers.size() + 1; i++)
            delete[] envp[i];
        delete[] envp;
    }
    else
    {
        for (size_t i = 0; i != params.size() + 1; i++)
            delete[] envp[i];
        delete[] envp;
    }
    for (size_t i = 0; i != 2; i++)
        delete[] argv[i];
    delete[] argv;
}

int CGI::control_fd(int &new_fd)
{
    if(fcntl(new_fd, F_SETFD, O_CLOEXEC) == -1 || fcntl(new_fd, F_SETFL, O_NONBLOCK) == -1)
        return (0);
    return (1);
}

void    CGI::dupEnv(std::map<std::string, std::string> &env)
{
    std::string query_parameter;
    std::map<std::string, std::string>::iterator it;
    int     count = 0;

    envp = new char*[env.size() + 1];
    for (it = env.begin(); it != env.end(); ++it)
    {
        query_parameter = it->first + "=" + it->second;
        envp[count] = new char [query_parameter.size() + 1];
        std::strcpy(envp[count], query_parameter.c_str());
        count++;
    }
    envp[count] = NULL;
}

int   CGI::makeProcess()
{
    if (pipe(fd_pipe) == -1 || !control_fd(fd_pipe[0]) || !control_fd(fd_pipe[1]))
    {
        std::cerr << "Pipe error." << strerror(errno) << std::endl;
        return (1);
    }
    envp = NULL;
    if (accept_method == "POST")
        dupEnv(headers);
    else
        dupEnv(params);
    argv = new char*[3];
    argv[0] = new char [20];
    if (root.substr(root.size() - 3) == ".py")
        std::strcpy(argv[0], "/usr/bin/python3");
    else if (root.substr(root.size() - 4) == ".php")
        std::strcpy(argv[0], "/usr/bin/php");
    else if (root.substr(root.size() - 3) == ".pl")
        std::strcpy(argv[0], "/usr/bin/perl");
    argv[1] = new char [root.size() + 1];
    std::strcpy(argv[1], root.c_str());
    argv[2] = NULL;
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Fork error. " << strerror(errno) << std::endl;
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        deleteArray();
        return (1);
    }
    if (pid == 0)
        make_execve();
    else
        deleteArray();
    return (0);
}

void    CGI::make_execve()
{
    if (accept_method == "POST")
    {
        std::string str(body.begin(), body.end());
        if (write(fd_pipe[1], str.c_str(), str.size()) == -1)
        {
            std::cerr << "Internal error: " << strerror(errno) << std::endl;
            deleteArray();
            exit (1);
        }

    }
    if (dup2(fd_pipe[1], STDOUT_FILENO) < 0 || dup2(fd_pipe[0], STDIN_FILENO) < 0)
    {
        std::cerr << "dup2 error. " << strerror(errno) << std::endl;
        close(fd_pipe[1]);
        close(fd_pipe[0]);
        deleteArray();
        exit (1);
    }
    if (close(fd_pipe[1]) == -1 || close(fd_pipe[0]) == -1)
    {
        std::cerr << "close error. " << strerror(errno) << std::endl;
        deleteArray();
        exit (1);
    }
    if (execve(argv[0], argv, envp) == -1)
	{
        std::cerr << "\033[31m" << "Execve error: " << "\033[0m" << strerror(errno) << std::endl;
        deleteArray();
        exit (1);
	}
}
