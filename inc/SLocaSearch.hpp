#ifndef SLOCASEARCH_HPP
#define SLOCASEARCH_HPP

#include "Location.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <vector>

typedef struct s_key_loca {
  std::string servername;
  std::string location;
  uint32_t    ip;
  uint16_t    port;
} t_key_loca;

template <>
struct std::less<t_key_loca>
{
    bool operator()(const t_key_loca &a, const t_key_loca &b) const { return (strcmp(a.location.GetOR(), b.location.GetOR()) < 0); }
};

class SLocaSearch {
public:
  static void addLocation(const Location &location) {
    t_key_loca  new_loca;
    std::list<std::string> listens;

    _llocations.push_back(location);
    if (!_defaultl)
      _defaultl = &_llocations.back();
    // TODO
    new_loca.location = location.getLocation();
    listens = location.getListens();
    for (std::list<std::string>::const_iterator it = listens.begin(); it != listens.end(); it++)
    {
      std::string lis = *it;
      std::string ip_s = "";
      std::string port_s = "";
      uint32_t  ip = 0;
      uint32_t  port = 0;

      if (lis.find(":"))
      {
        ip_s = lis.substr(0, lis.find((":")));
        port_s = lis.substr(lis.find((":")));
      }
      else
        port_s = lis;
      ip = ip2u32(ip_s);
      port = std::atoi(port_s.c_str());
      new_loca.ip = ip;
      new_loca.port = port;
      if (location.getServernames().size())
      {
        std::list<std::string> servernames;

        servernames = location.getServernames();
        for (std::list<std::string>::const_iterator it = servernames.begin(); it != servernames.end(); it++) {

        }
      }
      else
        _llocations.push_back()
    }
    //Create list of keys

  }

  static Location *search(t_key_loca key) {
    Location *result;
    // TODO
    return result;
  }

  static std::list<Location> getAllLocations() { return _llocations; }

private:
  static int loca_cmp(t_key_loca &a, t_key_loca &b) {
    return 0;
  }
  static uint32_t  ip2u32(std::string ip) {
    uint32_t  ret = 0;

    for (int i = 0; i < 4; i++)
    {
      ret += std::atoi(ip.c_str()) * std::pow(256, i);
      ip = ip.substr(ip.find("."));
    }
    return ret;
  }
  SLocaSearch() = default;
  ~SLocaSearch() = default;

  SLocaSearch(const SLocaSearch &) = delete;
  SLocaSearch &operator=(const SLocaSearch &) = delete;

  std::map<t_key_loca, Location *> _mlocations;
  std::map<t_key_loca, Location *> _mDefaults;
};

#endif // SLOCASEARCH_HPP
