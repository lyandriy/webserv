#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "AConfig.hpp"

class WebServer : public AConfig{
public:
  WebServer();
  ~WebServer();
  std::string getRoot() {throw rootNotAvailable(); };
  void setRoot(std::string root) { (void) root; throw rootNotAvailable(); };
  std::string getIndex() { throw indexNotAvailable(); };
  void setIndex(std::string index) { (void) index; throw indexNotAvailable(); };
  std::string getRedirection() { throw redirectionNotAvailable(); };
  void setRedirection(std::string redirection) { (void) redirection; throw redirectionNotAvailable(); };
protected:
private:
};

#endif // !WEBSERVER_HPP
