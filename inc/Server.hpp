#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServer.hpp"

class Server : public WebServer{
public:
  Server(const WebServer &other);
  ~Server();
  std::string getRoot();
  void setRoot(std::string root);
  std::string getIndex();
  void setIndex(std::string index);
  std::string getRedirection();
  void setRedirection(std::string redirection);
protected:
private:
  
};

#endif // !SERVER_HPP
