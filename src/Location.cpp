#include "../inc/Location.hpp"

std::ostream &operator<<( std::ostream& os , Location const&  other )
{
    return os << "---------\nLocation\n" << (Server &) other;
}

Location::Location(const Server &other) : Server(other) {}

Location::~Location() {}
