#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Server.hpp"

class Location;

std::ostream &operator<<( std::ostream& os , Location const&  other );

class Location : public Server {
public:
  Location(const Server &other);
  ~Location();

protected:
private:
};

#endif // !LOCATION_HPP
