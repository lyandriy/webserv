# include "../inc/Webserver.hpp"

CGI::CGI()
{
    this->root = "";
    this->uri = "";
}

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
    this->fd_file = other.fd_file;
    this->fd_pipe[0] = other.fd_pipe[0];
    this->fd_pipe[1] = other.fd_pipe[1];
    this->pid = other.pid;
    this->envp = other.envp;
    this->argv = other.argv;
    return (*this);
}

pid_t   CGI::makeProces(Response &response)
{
    std::map<std::string, std::string>::iterator it;
    int     count = 0;
    std::string query_parameter;

    if (pipe(fd_pipe) == -1)
    {
        std::cerr << "Pipe error." << std::endl;
        return (-2);
    }
    argv = new char*[params.size() + 1];
    for (it = params.begin(); it != params.end(); ++it)
    {
        query_parameter = it->first + "=" + it->second;
        argv[count] = new char [query_parameter.size() + 1];
        std::strcpy(argv[count], query_parameter.c_str());
        count++;
    }
    argv[count] = NULL;
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Fork error." << std::endl;
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        return (-2);
    }
    if (pid == 0)
        make_execve();
    close(fd_pipe[1]);
    //cuando termina el hijo,hacer delete
    fd_file = fd_pipe[0];//luego hay que sacaer lo que mide el pipe!!!!!!!!!!!!!!!
    return (fd_file);
}

void    CGI::make_execve()
{
    if (dup2(fd_pipe[1], STDOUT_FILENO) == -1 || dup2(fd_pipe[0], STDIN_FILENO) == -1)
    {
        std::cerr << "dup2 error." << std::endl;
        close(fd_pipe[1]);
        close(fd_pipe[0]);
        exit (-2);
    }
    close(fd_pipe[1]);
    close(fd_pipe[0]);
    
    if (execve(uri.c_str(), envp, argv) == -1)
	{
		std::cerr << "Execve error." << std::endl;
        exit (-2);
	}
}
