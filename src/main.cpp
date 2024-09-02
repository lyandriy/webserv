#include <exception>
#include <iostream>

#include "../inc/Location.hpp"
#include "../inc/Server.hpp"
#include "../inc/WebServer.hpp"

int main (int argc, char **args, char **envs) {
  (void) argc;
  (void) args;
  (void) envs;
  std::cout << "Hello Webserver!" << std::endl;
  {
    WebServer webServer;
    webServer.setErrorPath("testing");
    Server    server(webServer);
    server.setErrorPath("testing2");
    Location  loca(server);

    std::cout << "AA" << webServer.getErrorPath() << std::endl;
    std::cout << "BB" << server.getErrorPath() << std::endl;
    std::cout << "CC" << loca.getErrorPath() << std::endl;
    std::cout << "AA" << webServer.getErrorPath() << std::endl;
    std::cout << "BB" << server.getErrorPath() << std::endl;
    std::cout << "CC" << loca.getErrorPath() << std::endl;
  }
  return 0;
}
