#include "../inc/WebServer.hpp"

WebServer::WebServer(): AConfig() 
{
  this->_index = "asdad1111111111111111";
  std::cout << "WebServer created" << std::endl;
} 

WebServer::~WebServer()
{
  std::cout << "WebServer destroyed" << std::endl;
}


