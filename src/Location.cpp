#include "../inc/Location.hpp"

std::ostream &operator<<( std::ostream& os , Location const&  other )
{
    return os << "---------\nLocation * " << &other << "\n" << (Server &) other \
    << "_location: " << other.getLocation() << std::endl;
}

Location::Location(const Server &other) : Server(other) {}

Location::~Location() {}

std::string Location::getLocation() const { return _location; }

void Location::setLocation(std::string location) { this->_location = location; }
