#ifndef SLOCASEARCH_HPP
#define SLOCASEARCH_HPP

#include "Location.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <list>
#include <map>
#include <string>

typedef struct s_key_loca {
  std::string servername;
  std::string location;
  u_int32_t ip;
  u_int16_t port;
} t_key_loca;

namespace std {
template <> struct less<t_key_loca> {
  bool operator()(const t_key_loca &a, const t_key_loca &b) const {
    if (a.port != b.port)
      return false;
    if (a.ip && b.ip && a.ip != b.ip)
      return false;
    if (a.servername.length() && b.servername.length() &&
        std::strcmp(a.servername.c_str(), b.servername.c_str()))
      return false;
    return true;
  }
};
}; // namespace std

class SLocaSearch {
public:
  static void addLocation(const Location &location);
  static Location *search(t_key_loca key);
  static std::list<Location> getAllLocations();
  static u_int32_t ip2u32(std::string ip);
  SLocaSearch() {};
  ~SLocaSearch() {};
  static std::list<Location> _llocations;
  static std::map<t_key_loca, Location *> _mlocations;
  static std::map<t_key_loca, Location *> _mdefaults;
private:
};

#endif // SLOCASEARCH_HPP
