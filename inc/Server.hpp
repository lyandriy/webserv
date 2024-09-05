#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServer.hpp"

class Server : public WebServer {
public:
  Server(const WebServer &other);
  ~Server();
  std::string getRoot() const;
  void setRoot(std::string root);
  std::string getIndex() const;
  void setIndex(std::string index);
  std::string getRedirection() const;
  void setRedirection(std::string redirection);
  std::list<std::string> getServernames() const;
  void setServernames(std::list<std::string> servernames);
  std::list<std::string> getListens() const;
  void setListens(std::list<std::string> listens);
  std::string getLocation() const;
  void setLocation(std::string location);

protected:
private:
};

#endif // !SERVER_HPP
