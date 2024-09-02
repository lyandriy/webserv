
#include "../inc/Server.hpp"

Server::Server(const WebServer &other)
{
  *static_cast<AConfig *>(this) = static_cast<const AConfig &>(other);
  std::cout << "Server created" << std::endl;
}

Server::~Server()
{
  std::cout << "Server destroyed" << std::endl;
}

std::string Server::getRoot()
{
  return _root;
}

void Server::setRoot(std::string root)
{
  _root = root;
}

std::string Server::getIndex()
{
  return _index;
}

void Server::setIndex(std::string index)
{
  _index = index;
}

std::string Server::getRedirection()
{
  return _redirection;
}

void Server::setRedirection(std::string redirection)
{
  _redirection = redirection;
}
