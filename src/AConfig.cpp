#include "../inc/AConfig.hpp"

AConfig::AConfig()
    : _errorPath("./pages/errors"), _clientBodyMaxSizeBytes(1024 * 1024 * 4),
      _clientTimeOutMs(60 * 1000), _serverBodyMaxSizeBytes(1024 * 1024 * 4),
      _serverTimeOutMs(60 * 1000), _directoryListing(false),
      _directoryFile("./pages/list_dirs.html"), _cgiPythonEnable(false),
      _cgiPythonBinPath("./cgi-bins/python3"), _cgiHaskelEnable(false),
      _cgiHaskelBinPath("./cgi-bins/haskel"), _root("./pages/default"),
      _allowedMethods({"GET", "POST", "PUT", "DELETE"}) {}

AConfig::AConfig(const AConfig &other) { *this = other; }

AConfig &AConfig::operator=(const AConfig &other) {
  if (this == &other)
    return *this;
  this->_errorPath = other._errorPath;
  this->_clientBodyMaxSizeBytes = other._clientBodyMaxSizeBytes;
  this->_clientTimeOutMs = other._clientTimeOutMs;
  this->_serverBodyMaxSizeBytes = other._serverBodyMaxSizeBytes;
  this->_serverTimeOutMs = other._serverTimeOutMs;
  this->_directoryListing = other._directoryListing;
  this->_directoryFile = other._directoryFile;
  this->_cgiPythonEnable = other._cgiPythonEnable;
  this->_cgiPythonBinPath = other._cgiPythonBinPath;
  this->_cgiHaskelEnable = other._cgiHaskelEnable;
  this->_cgiHaskelBinPath = other._cgiHaskelBinPath;
  this->_root = other._root;
  this->_allowedMethods = other._allowedMethods;
  return *this;
}

std::string AConfig::getErrorPath() const { return _errorPath; }
void AConfig::setErrorPath(std::string path) { _errorPath = path; }
size_t AConfig::getClientMaxSizeBytes() const {
  return _clientBodyMaxSizeBytes;
}
void AConfig::setClientMaxSizeBytes(size_t size) {
  _clientBodyMaxSizeBytes = size;
}
size_t AConfig::getClientTimeOutMs() const {}
void AConfig::setClientTimeOutMs(size_t size) {}
size_t AConfig::getServerMaxSizeBytes() const {}
void AConfig::setServerMaxSizeBytes(size_t size) {}
size_t AConfig::getServerTimeOutMs() const {}
void AConfig::setServerTimeOutMs(size_t timeout) {}
bool AConfig::getDirectoryListing() const {}
void AConfig::setDirectoryListing(bool state) {}
std::string AConfig::getDirectoryFile() const {}
void AConfig::setDirectoryFile(std::string path) {}
bool AConfig::getCgiPythonEnable() const {}
void AConfig::setCgiPythonEnable(bool state) {}
std::string AConfig::getCgiPythonBinPath() const {}
void AConfig::setCgiPythonBinPath(std::string path) {}
bool AConfig::getCgiHaskelEnable() const {}
void AConfig::setCgiHaskelEnable(bool state) {}
std::string AConfig::getCgiHaskelBinPath() const {}
void AConfig::setCgiHaskelBinPath(std::string path) {}
std::string AConfig::getRoot() const {}
void AConfig::setRoot(std::string root) {}
std::list<std::string> AConfig::getAllowedMethods() const {}
void AConfig::setAllowedMethods(std::list<std::string>) {}
