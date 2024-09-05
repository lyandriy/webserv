#include <exception>
#include <iostream>

#include "../inc/Location.hpp"
#include "../inc/Server.hpp"
#include "../inc/WebServer.hpp"

int main(int argc, char **args, char **envs) {
  (void)argc;
  (void)args;
  (void)envs;
  std::cout << "Test1" << std::endl;
  {
    WebServer webServer;
    Server server(webServer);
    Location loca(server);

    std::cout << webServer << std::endl;
    std::cout << server << std::endl;
    std::cout << loca << std::endl;
    /*std::cout << "webServer index" << webServer.getIndex() << std::endl;*/
    /*std::cout << "webServer root" << webServer.getRoot() << std::endl;*/
    /*std::cout << "webServer redirection" << webServer.getRedirection() <<
     * std::endl;*/
    std::cout << "webServer errors" << webServer.getErrorPath() << std::endl;

    std::cout << "server index" << server.getIndex() << std::endl;
    std::cout << "server root" << server.getRoot() << std::endl;
    std::cout << "server redirection" << server.getRedirection() << std::endl;
    std::cout << "server errors" << server.getErrorPath() << std::endl;

    std::cout << "loca index" << loca.getIndex() << std::endl;
    std::cout << "loca root" << loca.getRoot() << std::endl;
    std::cout << "loca redirection" << loca.getRedirection() << std::endl;
    std::cout << "loca errors" << loca.getErrorPath() << std::endl;
  }
  std::cout << "Test2" << std::endl;
  {
    WebServer webServer;
    webServer.setErrorPath("/test1/error/path/webServer");
    Server server(webServer);
    server.setErrorPath("/test1/error/path/server");
    Location loca(server);

    /*std::cout << "webServer index" << webServer.getIndex() << std::endl;*/
    /*std::cout << "webServer root" << webServer.getRoot() << std::endl;*/
    /*std::cout << "webServer redirection" << webServer.getRedirection() <<
     * std::endl;*/
    std::cout << "webServer errors" << webServer.getErrorPath() << std::endl;

    std::cout << "server index" << server.getIndex() << std::endl;
    std::cout << "server root" << server.getRoot() << std::endl;
    std::cout << "server redirection" << server.getRedirection() << std::endl;
    std::cout << "server errors" << server.getErrorPath() << std::endl;

    std::cout << "loca index" << loca.getIndex() << std::endl;
    std::cout << "loca root" << loca.getRoot() << std::endl;
    std::cout << "loca redirection" << loca.getRedirection() << std::endl;
    std::cout << "loca errors" << loca.getErrorPath() << std::endl;
  }
  std::cout << "Test3" << std::endl;
  {
    WebServer webServer;
    Server server(webServer);
    Location loca(server);

    /*std::cout << "webServer index" << webServer.getIndex() << std::endl;*/
    /*std::cout << "webServer root" << webServer.getRoot() << std::endl;*/
    /*std::cout << "webServer redirection" << webServer.getRedirection() <<
     * std::endl;*/
    std::cout << "webServer errors" << webServer.getErrorPath() << std::endl;

    std::cout << "server index" << server.getIndex() << std::endl;
    std::cout << "server root" << server.getRoot() << std::endl;
    std::cout << "server redirection" << server.getRedirection() << std::endl;
    std::cout << "server errors" << server.getErrorPath() << std::endl;

    std::cout << "loca index" << loca.getIndex() << std::endl;
    std::cout << "loca root" << loca.getRoot() << std::endl;
    std::cout << "loca redirection" << loca.getRedirection() << std::endl;
    std::cout << "loca errors" << loca.getErrorPath() << std::endl;
  }
  return 0;
}
