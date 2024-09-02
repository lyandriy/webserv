#include "../inc/Location.hpp"

Location::Location(const Server &other): Server(other)
{
  std::cout << "Location created" << std::endl;
}

Location::~Location()
{
  std::cout << "Location destroyed" << std::endl;
}
