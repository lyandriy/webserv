#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include <cstddef>
#include <list>
#include <string>

class AConfig {
public:
  AConfig();
  AConfig(const AConfig &other);
  AConfig &operator=(const AConfig &other);
  virtual ~AConfig();

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
  std::string getRoot() const;
  void setRoot(std::string root);
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
  std::string _root;
  std::list<std::string> _allowedMethods;
};

#endif // !ACONFIG_HPP
