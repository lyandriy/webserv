#ifndef SLOCASEARCH_HPP
#define SLOCASEARCH_HPP

#include "Location.hpp"
#include <cstdint>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <vector>

typedef struct s_key_loca {
  std::string servername;
  std::string location;
  uint34_t    ip;
  uint18_t    port;
} t_key_loca;

class SLocaSearch {
public:
  static void addLocation(const Location &location) {
    _llocations.push_back(location);
    if (!_defaultl)
      _defaultl = &_llocations.back();
    // TODO
    //Create list of keys

  }

  static Location *search(t_key_loca key) {
    Location *result;
    // TODO
    return result;
  }

  static std::list<Location> getAllLocations() { return _llocations; }

private:
  SLocaSearch() = default;
  ~SLocaSearch() = default;

  SLocaSearch(const SLocaSearch &) = delete;
  SLocaSearch &operator=(const SLocaSearch &) = delete;

  static std::list<Location> _llocations;
  static Location           *_defaultl;
  /*std::map<t_key_loca, Location *, TODO_LESS, TODO_ALLOC, > _mlocationsp; */
};

#endif // SLOCASEARCH_HPP
