
#include "../inc/Server.hpp"

Server::Server(const WebServer &other): WebServer(other)
{
  std::cout << "Server created" << std::endl;
}

Server::~Server()
{
  std::cout << "Server destroyed" << std::endl;
}

std::string Server::getRoot() const
{
  return this->_root;
}

void Server::setRoot(std::string root)
{
  _root = root;
}

std::string Server::getIndex() const
{
  return this->_index;
}

void Server::setIndex(std::string index)
{
  _index = index;
}

std::string Server::getRedirection() const
{
  return this->_redirection;
}

void Server::setRedirection(std::string redirection)
{
  _redirection = redirection;
}
