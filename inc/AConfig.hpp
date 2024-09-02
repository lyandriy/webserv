#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include <cstddef>
#include <exception>
#include <list>
#include <string>
#include <iostream>

class AConfig {
public:
  AConfig();
  AConfig(const AConfig &other);
  AConfig &operator=(const AConfig &other);
  virtual ~AConfig() = 0;
  virtual std::string getRoot() = 0;
  virtual void setRoot(std::string root) = 0;
  virtual std::string getIndex() = 0;
  virtual void setIndex(std::string index) = 0;
  virtual std::string getRedirection() = 0;
  virtual void setRedirection(std::string redirection) = 0;
  std::string getErrorPath() const;
  void setErrorPath(std::string path);
  size_t getClientMaxSizeBytes() const;
  void setClientMaxSizeBytes(size_t size);
  size_t getClientTimeOutMs() const;
  void setClientTimeOutMs(size_t size);
  size_t getServerMaxSizeBytes() const;
  void setServerMaxSizeBytes(size_t size);
  size_t getServerTimeOutMs() const;
  void setServerTimeOutMs(size_t timeout);
  bool getDirectoryListing() const;
  void setDirectoryListing(bool state);
  std::string getDirectoryFile() const;
  void setDirectoryFile(std::string path);
  bool getCgiPythonEnable() const;
  void setCgiPythonEnable(bool state);
  std::string getCgiPythonBinPath() const;
  void setCgiPythonBinPath(std::string path);
  bool getCgiHaskelEnable() const;
  void setCgiHaskelEnable(bool state);
  std::string getCgiHaskelBinPath() const;
  void setCgiHaskelBinPath(std::string path);
  std::list<std::string> getAllowedMethods() const;
  void setAllowedMethods(std::list<std::string>);
protected:
  std::string _errorPath;
  size_t _clientBodyMaxSizeBytes;
  size_t _clientTimeOutMs;
  size_t _serverBodyMaxSizeBytes;
  size_t _serverTimeOutMs;
  bool _directoryListing;
  std::string _directoryFile;
  bool _cgiPythonEnable;
  std::string _cgiPythonBinPath;
  bool _cgiHaskelEnable;
  std::string _cgiHaskelBinPath;
  std::list<std::string> _allowedMethods;
  std::string _root;
  std::string _index;
  std::string _redirection;
  class rootNotAvailable : public std::exception { virtual const char *what () const throw(); };
  class indexNotAvailable : public std::exception { virtual const char *what () const throw(); };
  class redirectionNotAvailable : public std::exception { virtual const char *what () const throw(); };
private:
};

#endif // !ACONFIG_HPP
