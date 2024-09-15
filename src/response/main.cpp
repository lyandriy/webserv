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
    std::vector<Request> requests;
	std::vector<Response> response;
    server = recv_conf(argc, argv);//recibe conf de servidres
    
    struct pollfd* pfds = new pollfd[BACKLOG];
    SocketManager   socketManager = SocketManager(pfds, server);//abre los socket para cada puerto

    while (true)
    {
        ready = poll(pfds, socketManager.getSockNum(), -1);//monitorear si hay algun cliente
        socketManager.error(ready);
        socketManager.AcceptClient(pfds, ready);//comprueba si hay un cliente y lo acepta
        socketManager.recvRequest(pfds, requests);//recibe mensajes de request
        //crear response
        //abrir el archivo a enviar y meterlo en pollfd
        //crear un bucle para ver los archivos con revent de POLLOUT y hacer send
    }
}