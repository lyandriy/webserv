# include "../inc/Webserver.hpp"

std::vector<Server>    &recv_conf(int argc, char **argv)
{
    try
    {
        if (argc == 2)
        {
            std::vector<Server> server;
            Parser  parser(argv[1]);
            server = parser.conf_file();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
		exit (1);
    }
}

int main(int argc, char **argv)
{
    std::vector<Server> server;
    server = recv_conf(argc, argv);//recibe conf de servidres
    
    struct pollfd* fds = new pollfd[BACKLOG];
    SocketManager   socketM = SocketManager(fds, server);//abre los socket para cada puerto

    
}