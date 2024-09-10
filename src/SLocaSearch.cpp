#include "../inc/SLocaSearch.hpp"
#include <iostream>
#include <ostream>
#include <string>

std::list<Location> SLocaSearch::_llocations;
std::map<t_key_loca, Location *> SLocaSearch::_mlocations;
std::map<t_key_loca, Location *> SLocaSearch::_mdefaults;

void SLocaSearch::addLocation(const Location &location) {
  t_key_loca new_loca;
  Location *aux;
  std::list<std::string> listens;

  SLocaSearch::_llocations.push_back(location);
  new_loca.location = location.getLocation();
  listens = location.getListens();
  for (std::list<std::string>::const_iterator it = listens.begin();
       it != listens.end(); it++) {
    std::string lis = *it;
    std::string ip_s = "";
    std::string port_s = "";
    u_int32_t ip = 0;
    u_int32_t port = 0;

    if (lis.find(":") != std::string::npos) {
      ip_s = lis.substr(0, lis.find((":")));
      port_s = lis.substr(lis.find((":")) + 1 );
    } else
      port_s = lis;
    if (ip_s.length())
      ip = ip2u32(ip_s);
    port = std::atoi(port_s.c_str());
    new_loca.ip = ip;
    new_loca.port = port;
    if (location.getServernames().size()) {
      std::list<std::string> servernames;
      servernames = location.getServernames();
      for (std::list<std::string>::const_iterator it = servernames.begin();
           it != servernames.end(); it++) {
        new_loca.servername = *it;
        aux = &_llocations.back();
        _mlocations.insert(std::pair<t_key_loca, Location *>(new_loca, aux));
      }
    } else {
      new_loca.servername = "";
      aux = &*_llocations.end();
        aux = &_llocations.back();
      _mlocations.insert(std::pair<t_key_loca, Location *>(new_loca, aux));
    }
  }
}

Location *SLocaSearch::search(t_key_loca key) {
  Location *result;
  result = _mlocations.find(key)->second;
  // TODO
  return result;
}

std::list<Location> SLocaSearch::getAllLocations() { return _llocations; }

u_int32_t SLocaSearch::ip2u32(std::string ip) {
  u_int32_t ret = 0;

  for (int i = 0; i < 4; i++) {
    ret += std::atoi(ip.c_str()) * std::pow(256, i);
    ip = ip.substr(ip.find(".") + 1);
  }
  return ret;
}
