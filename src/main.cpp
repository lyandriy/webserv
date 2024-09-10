#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <ostream>
#include <string>

#include "../inc/Location.hpp"
#include "../inc/SLocaSearch.hpp"
#include "../inc/Server.hpp"
#include "../inc/WebServer.hpp"

int main(void) {
  std::cout << "Test 1 SLocaSearch" << std::endl;
  {
    std::list<std::string> test;
    WebServer webServer;
    Server server(webServer);
    Location loca1(server);
    test.push_back("192.168.1.1:80");
    loca1.setListens(test);
    Location loca2(server);
    test.push_back("192.168.10.1:80");
    loca2.setListens(test);
    Location loca3(server);
    Location loca4(server);
    Location loca5(server);

    try {
      SLocaSearch::addLocation(loca1);
      SLocaSearch::addLocation(loca2);

    } catch (const std::exception &e) {
      std::cout << e.what() << std::endl;
    }

    for (std::list<Location>::iterator it = SLocaSearch::_llocations.begin();
         it != SLocaSearch::_llocations.end(); it++)
      std::cout << *it << std::endl;
    for (std::map<t_key_loca, Location *>::iterator it =
             SLocaSearch::_mlocations.begin();
         it != SLocaSearch::_mlocations.end(); it++) {
      std::cout << "IP" << it->first.ip << std::endl;
      std::cout << "port " << it->first.port << std::endl;
      std::cout << "server " << it->first.servername << std::endl;
      std::cout << "location" << it->first.location << std::endl;
      std::cout << "Location * " << it->second << std::endl;
    }

    Location *t = SLocaSearch::search((t_key_loca) {
      .servername = "",
      .location = "/",
      .ip = 16885952,
      .port = 80,
    });
    std::cout << "res" << t << std::endl; 
  }
  return 0;
}
