#include "../inc/AConfig.hpp"
#include <list>
#include <ostream>
#include <string>

std::ostream &operator<<(std::ostream &os,
                         std::list<std::string> const &other) {
  os << std::endl;
  for (std::list<std::string>::const_iterator it = other.begin();
       it != other.end(); it++)
    os << "\t" << *it << std::endl;
  return os;
}

std::ostream &operator<<(std::ostream &os, AConfig const &other) {
  return os << "Printing AConfig " << &other << "\n"
            << "_errorPath: " << other.getErrorPath() << "\n"
            << "_clientBodyMaxSizeBytes: " << other.getClientMaxSizeBytes()
            << "\n"
            << "_clientTimeOutMs: " << other.getClientTimeOutMs() << "\n"
            << "_serverBodyMaxSizeBytes: " << other.getServerMaxSizeBytes()
            << "\n"
            << "_serverTimeOutMs: " << other.getServerTimeOutMs() << "\n"
            << "_directoryListing: " << other.getDirectoryListing() << "\n"
            << "_directoryFile: " << other.getDirectoryFile() << "\n"
            << "_cgiPythonEnable: " << other.getCgiPythonEnable() << "\n"
            << "_cgiPythonBinPath: " << other.getCgiPythonBinPath() << "\n"
            << "_cgiHaskelEnable: " << other.getCgiHaskelEnable() << "\n"
            << "_cgiHaskelBinPath: " << other.getCgiHaskelBinPath() << "\n"
            << "_allowedMethods: " << other.getAllowedMethods() << std::endl;
}

AConfig::AConfig()
    : _errorPath("./pages/errors"), _clientBodyMaxSizeBytes(1024 * 1024 * 4),
      _clientTimeOutMs(60 * 1000), _serverBodyMaxSizeBytes(1024 * 1024 * 4),
      _serverTimeOutMs(60 * 1000), _directoryListing(false),
      _directoryFile("./pages/list_dirs.html"), _cgiPythonEnable(false),
      _cgiPythonBinPath("./cgi-bins/python3"), _cgiHaskelEnable(false),
      _cgiHaskelBinPath("./cgi-bins/haskel"), _location("/"),
      _root("/var/www/html"), _index("index.html"), _redirection("") {
  _allowedMethods.push_back("GET");
  _allowedMethods.push_back("POST");
  _allowedMethods.push_back("PUT");
  _allowedMethods.push_back("DELETE");
  _listens.push_back("80");
}

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
  this->_allowedMethods = other._allowedMethods;
  this->_root = other._root;
  this->_index = other._index;
  this->_redirection = other._redirection;
  this->_servernames = other._servernames;
  this->_listens = other._listens;
  this->_location = other._location;
  return *this;
}

AConfig::~AConfig() {}

std::string AConfig::getErrorPath() const { return this->_errorPath; }
void AConfig::setErrorPath(std::string path) { _errorPath = path; }
size_t AConfig::getClientMaxSizeBytes() const {
  return this->_clientBodyMaxSizeBytes;
}
void AConfig::setClientMaxSizeBytes(size_t size) {
  _clientBodyMaxSizeBytes = size;
}
size_t AConfig::getClientTimeOutMs() const { return this->_clientTimeOutMs; }

void AConfig::setClientTimeOutMs(size_t size) { _clientTimeOutMs = size; }

size_t AConfig::getServerMaxSizeBytes() const {
  return this->_serverBodyMaxSizeBytes;
}

void AConfig::setServerMaxSizeBytes(size_t size) {
  _serverBodyMaxSizeBytes = size;
}

size_t AConfig::getServerTimeOutMs() const { return this->_serverTimeOutMs; }

void AConfig::setServerTimeOutMs(size_t timeout) { _serverTimeOutMs = timeout; }

bool AConfig::getDirectoryListing() const { return this->_directoryListing; }

void AConfig::setDirectoryListing(bool state) { _directoryListing = state; }

std::string AConfig::getDirectoryFile() const { return this->_directoryFile; }

void AConfig::setDirectoryFile(std::string path) { _directoryFile = path; }

bool AConfig::getCgiPythonEnable() const { return this->_cgiPythonEnable; }

void AConfig::setCgiPythonEnable(bool state) { _cgiPythonEnable = state; }

std::string AConfig::getCgiPythonBinPath() const {
  return this->_cgiPythonBinPath;
}

void AConfig::setCgiPythonBinPath(std::string path) {
  _cgiPythonBinPath = path;
}

bool AConfig::getCgiHaskelEnable() const { return this->_cgiHaskelEnable; }

void AConfig::setCgiHaskelEnable(bool state) { _cgiHaskelEnable = state; }

std::string AConfig::getCgiHaskelBinPath() const {
  return this->_cgiHaskelBinPath;
}

void AConfig::setCgiHaskelBinPath(std::string path) {
  _cgiHaskelBinPath = path;
}

std::list<std::string> AConfig::getAllowedMethods() const {
  return this->_allowedMethods;
}

void AConfig::setAllowedMethods(std::list<std::string> methods) {
  _allowedMethods = methods;
}

const char *AConfig::rootNotAvailable::what() const throw() {
  return "Root not available in this class";
}

const char *AConfig::indexNotAvailable::what() const throw() {
  return "Index not available in this class";
}

const char *AConfig::redirectionNotAvailable::what() const throw() {
  return "Redirection not available in this class";
}

const char *AConfig::servernamesNotAvailable::what() const throw() {
  return "Servernames not available in this class";
}

const char *AConfig::locationNotAvailable::what() const throw() {
  return "Location not available in this class";
}

const char *AConfig::listensNotAvailable::what() const throw() {
  return "Listens not available in this class";
}
