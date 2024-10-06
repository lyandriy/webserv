# include "../inc/Webserver.hpp"

std::vector<Server>    recv_conf(int argc, char **argv)
{
    std::vector<Server> server;
    try
    {
        if (argc == 2)
        {
            Parser  parser(argv[1]);
            server = parser.conf_file();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
		exit (1);
    }
    return (server);
}

int main(int argc, char **argv)
{
    int ready;
    std::vector<Server> server;
    server = recv_conf(argc, argv);//recibe conf de servidres
    
    struct pollfd* pfds = new pollfd[BACKLOG];
    SocketManager   socketManager = SocketManager(pfds, server);//abre los socket para cada puerto

    while (true)
    {
        if ((ready = poll(pfds, socketManager.getSockNum(), 1000)) == -1)//monitorear si hay algun cliente
             std::cerr << "Error: poll error." << std::endl;
        socketManager.acceptClient(pfds);//comprueba si hay un cliente y lo acepta
        socketManager.recvRequest(pfds, server);//recibe mensajes de request
        std::cout << "\033[33m" << "main" <<  "\033[0m" << std::endl;
        socketManager.sendResponse(pfds);//responder al cliente
        //ver que pasa si socket se sierra solo por algun error
    }
}

