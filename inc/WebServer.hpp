#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "AConfig.hpp"

class WebServer;

std::ostream &operator<<(std::ostream &os, WebServer const &other);

class WebServer : public AConfig {
public:
  WebServer();
  ~WebServer();
  std::string getRoot() const { throw rootNotAvailable(); };
  void setRoot(std::string root) {
    (void)root;
    throw rootNotAvailable();
  };
  std::string getIndex() const { throw indexNotAvailable(); };
  void setIndex(std::string index) {
    (void)index;
    throw indexNotAvailable();
  };
  std::string getRedirection() const { throw redirectionNotAvailable(); };
  void setRedirection(std::string redirection) {
    (void)redirection;
    throw redirectionNotAvailable();
  };
  std::list<std::string> getServernames() const { throw servernamesNotAvailable(); };
  void setServernames(std::list<std::string> servernames) {
    (void)servernames;
    throw servernamesNotAvailable();
  };
  std::list<std::string> getListens() const { throw listensNotAvailable(); };
  void setListens(std::list<std::string> redirection) {
    (void)redirection;
    throw redirectionNotAvailable();
  };
  std::string getLocation() const { throw locationNotAvailable(); };
  void setLocation(std::string location) {
    (void)location;
    throw locationNotAvailable();
  };

protected:
private:
};

#endif // !WEBSERVER_HPP
