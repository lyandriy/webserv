# include "../inc/Webserver.hpp"

//std::cout << "\033[31m" << client << " POLLOUT " << pfds[client].fd << "\033[0m" << std::endl;

void    closeWebserv(int signal)
{
    if (signal == 2)
        std::cout << "\nReceived Interrupt Signal. Shutting down the web server...\n";
    else
        std::cout << "\nReceived signal " << signal << " Shutting down the web server...\n";
    std::cout << "Web server stopped." << std::endl;
    std::cout << "Thank you for using the service.\n";
    exit(0);
}

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

            signal(SIGINT, closeWebserv);
            while (true)
            {
                if (poll(pfds, socketManager.getSockNum(), 1000) == -1)//monitorear si hay algun cliente
                    std::cerr << "Error: poll error." << std::endl;    
                for (int i = 0; i < socketManager.getSockNum(); ++i)
                {
                    std::cout << "socket " << i << ": ";
                    std::cout << "  fd: " << pfds[i].fd << "; ";
                    std::cout << "  events: " << pfds[i].events << "; ";
                    std::cout << "  revents: " << pfds[i].revents << std::endl;
                    socketManager.check_revent(i);
                } 
                if (first_poll > 0)
                {
                    socketManager.acceptClient();//comprueba si hay un cliente y lo acepta
                    socketManager.reventPOLLIN(server);//recibe mensajes de request
                    socketManager.sendResponse();//responder al cliente
                    socketManager.CommonGatewayInterface();  
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

//gestionar cuando se descnecta el cliente. se rompe totalmente
//recvisar close_move_pfd