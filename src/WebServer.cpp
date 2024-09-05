#include "../inc/WebServer.hpp"

std::ostream &operator<<( std::ostream& os , WebServer const&  other )
{
  return os << "---------\nWebServer\n" << (AConfig &) other;
}

WebServer::WebServer() {}

WebServer::~WebServer() {}
