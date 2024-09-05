
#include "../inc/Server.hpp"

std::ostream &operator<<( std::ostream& os , Server const&  other )
{
  return os << "---------\nServer\n" <<  (AConfig &) other << \
  "_root: " << other.getRoot() << "\n" << \
  "_redirection: " << other.getRedirection() << "\n" << \
  "_servernames: " << other.getServernames() << "\n" << \
  "_listens: " << other.getListens() << "\n" << \
  "_location: " << other.getLocation() << "\n" << \
  "_index: " << other.getIndex() << std::endl;
}

Server::Server(const WebServer &other) : WebServer(other) {}

Server::~Server() {}

std::string Server::getRoot() const { return this->_root; }

void Server::setRoot(std::string root) { _root = root; }

std::string Server::getIndex() const { return this->_index; }

void Server::setIndex(std::string index) { _index = index; }

std::string Server::getRedirection() const { return this->_redirection; }

void Server::setRedirection(std::string redirection) {
  _redirection = redirection;
}

std::list<std::string> Server::getServernames() const { return _servernames; }

void Server::setServernames(std::list<std::string> servernames) {
  this->_servernames = servernames;
}

std::list<std::string> Server::getListens() const { return _listens; }

void Server::setListens(std::list<std::string> listens) {
  this->_listens = listens;
}

std::string Server::getLocation() const { return _location; }

void Server::setLocation(std::string location) { this->_location = location; }
