# include "../inc/Webserver.hpp"

//std::cout << "\033[33m" << " MAIN " <<  "\033[0m" << std::endl;

int main(int argc, char **argv)
{
    int first_poll = 0;
    std::vector<Server> server;
    struct pollfd pfds[BACKLOG];
    
    try
    {
        if (argc > 2)
            throw std::runtime_error("Too many arguments.");
        else
        {
            Parser  parser(argv[1]);
            server = parser.conf_file();
            SocketManager   socketManager = SocketManager(pfds, server);//abre los socket para cada puerto

            while (true)
            {
                if (poll(pfds, socketManager.getSockNum(), 1000) == -1)//monitorear si hay algun cliente
                    std::cerr << "Error: poll error." << std::endl;
                if (first_poll > 0)
                {
                    socketManager.acceptClient(pfds);//comprueba si hay un cliente y lo acepta
                    socketManager.reventPOLLIN(pfds, server);//recibe mensajes de request
                    socketManager.sendResponse(pfds);//responder al cliente
                    socketManager.CommonGatewayInterface(pfds);
                }
                if (first_poll == 0)
                    first_poll++;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}

/*for (int i = 0; i < socketManager.getSockNum(); ++i) {
    std::cout << "socket " << i << ": ";
    std::cout << "  fd: " << pfds[i].fd << "; ";
    std::cout << "  events: " << pfds[i].events << "; ";
    std::cout << "  revents: " << pfds[i].revents << std::endl;
}*/