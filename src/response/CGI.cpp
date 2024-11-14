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
    std::cout << "\033[34m" << " CGI " <<  "\033[0m" << std::endl;
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
    std::cout << "HOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
}

int   CGI::makeProcess()
{
    std::cout << "\033[35m" << " Make process" <<  "\033[0m" << std::endl;
    std::map<std::string, std::string>::iterator it;
    int     count = 0;
    std::string query_parameter;

    if (pipe(fd_pipe) == -1)
    {
        std::cerr << "Pipe error." << std::endl;
        return (0);
    }
    //crear el argv
    std::cout << "params.size()" << params.size() << std::endl;
    envp = new char*[params.size() + 1];
    for (it = params.begin(); it != params.end(); ++it)
    {
        std::cout << "\033[34m" << " it->first " << it->first << " it->second " << it->second <<  "\033[0m" << std::endl;
        query_parameter = it->first + "=" + it->second;
        envp[count] = new char [query_parameter.size() + 1];
        std::strcpy(envp[count], query_parameter.c_str());
        count++;
    }
    envp[count] = NULL;
    std::cout << count << std::endl;
    //crer el envp
    argv = new char*[2];
    argv[0] = new char [root.size()];
    std::strcpy(argv[0], root.c_str());
    argv[1] = NULL;
    //fork
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Fork error." << std::endl;
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        deleteArray();
        return (0);
    }
    if (pid == 0)
        make_execve();
    else
    {
        close(fd_pipe[1]);
        deleteArray();
    }
    std::cout << "\033[36m" << " termino make process "  <<  "\033[0m" << std::endl;
    return (1);
}

void    CGI::make_execve()
{
    printArgumentsAndEnvironment();
    if (dup2(fd_pipe[1], STDOUT_FILENO) == -1 || dup2(fd_pipe[0], STDIN_FILENO) == -1)
    {
        std::cerr << "dup2 error." << std::endl;
        close(fd_pipe[1]);
        close(fd_pipe[0]);
        deleteArray();
        exit (01);
    }
    close(fd_pipe[1]);
    close(fd_pipe[0]);
    if (execve(root.c_str(), argv, envp) == -1)
	{
		std::cerr << "Execve error." << std::endl;
        std::cerr << "Execve error: " << strerror(errno) << std::endl;
        deleteArray();
        exit (1);
	}
}

void CGI::printArgumentsAndEnvironment() {
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
