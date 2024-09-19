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
    int ready;
    std::vector<Server> server;
    //std::map<int, Request> requests;
	//std::vector<Response> response;
    server = recv_conf(argc, argv);//recibe conf de servidres
    
    struct pollfd* pfds = new pollfd[BACKLOG];
    SocketManager   socketManager = SocketManager(pfds, server);//abre los socket para cada puerto

    while (true)
    {
        if ((ready = poll(pfds, socketManager.getSockNum(), -1)) == -1)//monitorear si hay algun cliente
             std::cerr << "Error: poll error." << std::endl;
        socketManager.AcceptClient(pfds, ready);//comprueba si hay un cliente y lo acepta
        socketManager.recvRequest(pfds, server);//recibe mensajes de request
        socketManager.sendResponse(pfds);//responder al cliente
        //ver que pasa si socket se sierra solo por algun error
    }
}