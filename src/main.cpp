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
            if (argc == 1)
            {
                Parser  parser("serverHTML/config_default");
                server = parser.conf_file();
            }
            else
            {
                Parser  parser(argv[1]); 
                server = parser.conf_file();
            }
            SocketManager   socketManager = SocketManager(pfds, server);//abre los socket para cada puerto
            signal(SIGINT, closeWebserv);
            while (true)
            {
                if (poll(pfds, BACKLOG, 1000) == -1)//monitorear si hay algun cliente
                    std::cerr << "Error: poll error." << std::endl;    
                for (int i = 0; i < BACKLOG; ++i)
                {
                    if (pfds[i].fd != -1)
                    {
                        std::cout << "socket " << i << ": ";
                        std::cout << "  fd: " << pfds[i].fd << "; ";
                        std::cout << "  events: " << pfds[i].events << "; ";
                        std::cout << "  revents: " << pfds[i].revents << std::endl;
                    }
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

//recvisar close_move_pfd
//response con cgi