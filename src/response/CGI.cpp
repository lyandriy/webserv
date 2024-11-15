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
    this->body = other.body;
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
    this->body = response.getBody();
    this->pid = -2;
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

int CGI::getFd() const
{
    return (this->fd_pipe[0]);
}

pid_t   CGI::getPid() const
{
    return (this->pid);
}

void    CGI::deleteArray()
{
    for (size_t i = 0; i != params.size() + 1; i++)
        delete[] envp[i];
    delete[] envp;
    for (size_t i = 0; i != 2; i++)
        delete[] argv[i];
    delete[] argv;
}

int   CGI::makeProcess()
{
    std::map<std::string, std::string>::iterator it;
    int     count = 0;
    std::string query_parameter;

    if (pipe(fd_pipe) == -1)
    {
        std::cerr << "Pipe error." << strerror(errno) << std::endl;
        return (1);
    }
    //crear el argv
    envp = new char*[params.size() + 1];
    for (it = params.begin(); it != params.end(); ++it)
    {
        query_parameter = it->first + "=" + it->second;
        envp[count] = new char [query_parameter.size() + 1];
        std::strcpy(envp[count], query_parameter.c_str());
        count++;
    }
    envp[count] = NULL;
    //crer el envp
    argv = new char*[3];
    argv[0] = new char [20];
    std::strcpy(argv[0], "/bin/bash");
    argv[1] = new char [root.size()];
    std::strcpy(argv[1], root.c_str());
    argv[2] = NULL;
    //fork
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Fork error. " << strerror(errno) << std::endl;
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        deleteArray();
        return (1);//1 es error
    }
    if (pid == 0)
        make_execve();
    else
    {
        close(fd_pipe[1]);
        deleteArray();
    }
    //std::cout << "\033[33m" << " pid " << pid <<  "\033[0m" << std::endl;
    return (0);
}

void    CGI::make_execve()
{
    //printArgumentsAndEnvironment();
    //std::cout << "MI PIPE " << fd_pipe[1] << fd_pipe[0] << std::endl;
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
    std::cerr << "close error. " << std::endl;
    /*write(STDOUT_FILENO, "hola", 4);
    char buffer[4];
    read(STDIN_FILENO, buffer, 4);
    std::cerr << "aaaaaaaaaaaaaaaaaaaaaaaaaaaa " << buffer << std::endl;*/
    if (execve("/bin/bash", argv, envp) == -1)
	{
        std::cerr << "Execve error: " << strerror(errno) << std::endl;
        deleteArray();
        exit (1);
	}
}

void CGI::printArgumentsAndEnvironment()
{
    int i = 0;
    while (argv[i] != NULL)
    {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
        i++;
    }

    // Imprimir los valores de envp
    int j = 0;
    while (envp[j] != NULL)
    {
        std::cout << "envp[" << j << "] = " << envp[j] << std::endl;
        j++;
    }

    // Imprimir los valores de root y uri
    std::cout << "\nroot: " << root << std::endl;
}
