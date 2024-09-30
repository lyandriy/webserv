# include "../inc/Webserver.hpp"

Response::Response(){}

Response::~Response(){}

Response::Response(const Response &other){
    *this = other;
}

Response::Response(Location &location, Request &request){
    this->root = location.getRoot();
    this->index = location.getIndex();
    this->redirection = location.getRedirection();
    this->error_page = location.getErrorPage();
    this->cgi = location.getCGI();
    this->autoindex = location.getAutoindex();
    
    this->listen.sin_family = AF_INET;
    this->listen.sin_port = htons(request.get_port());
    this->listen.sin_addr.s_addr = INADDR_ANY;
    this->accept_method = request.get_method();
    this->error_code = request.get_error_code();
    this->uri = request.get_uri();
    this->protocol = request.get_protocol();
    this->host = request.get_host();
    this->help_message = request.get_help_message();
    this->headers = request.get_headers();
    this->_pos_file_response = -1;
}

Response::Response(Server &server, Request &request){
    this->root = server.getRoot();
    this->index = server.getIndex();
    this->redirection = server.getRedirection();
    this->error_page = server.getErrorPage();
    this->autoindex = server.getAutoindex();
    this->cgi = server.getCGI();
    
    this->listen.sin_family = AF_INET;
    this->listen.sin_port = htons(request.get_port());
    this->listen.sin_addr.s_addr = INADDR_ANY;
    this->accept_method = request.get_method();
    this->error_code = request.get_error_code();
    this->uri = request.get_uri();
    this->protocol = request.get_protocol();
    this->host = request.get_host();
    this->help_message = request.get_help_message();
    this->headers = request.get_headers();
    this->_pos_file_response = -1;
}

Response::Response(Request &request)
{
    this->listen.sin_family = AF_INET;
    this->listen.sin_port = htons(request.get_port());
    this->listen.sin_addr.s_addr = INADDR_ANY;
    this->help_message = request.get_help_message();
    if (request.get_error_code() == BAD_REQUEST)
        this->root = ROOT_BAD_REQUEST;
    else if (request.get_error_code() == METHOD_NOT_ALLOWED)
        this->root = ROOT_METHOD_NOT_ALLOWED;
    else if (request.get_error_code() == CONTENT_TOO_LARGE)
        this->root = ROOT_CONTENT_TOO_LARGE;
    else if (request.get_error_code() == URI_TOO_LONG)
        this->root = ROOT_URI_TOO_LONG;
    else if (request.get_error_code() == SERVICE_UNAVAIBLE)
        this->root = ROOT_SERVICE_UNAVAIBLE;
    else if (request.get_error_code() == HTTP_VERSION_NOT_SUPPORTED)
        this->root = ROOT_HTTP_VERSION_NOT_SUPPORTED;
    this->_pos_file_response = -1;
}

Response &Response::operator=(const Response &other){
    this->root = other.root;
    this->index = other.index;
    this->redirection = other.redirection;
    this->error_page = other.error_page;
    this->autoindex = other.autoindex;
    this->cgi = other.cgi;
    this->listen = other.listen;
    this->accept_method = other.accept_method;
    this->error_code = other.error_code;
    this->uri = other.uri;
    this->protocol = other.protocol;
    this->host = other.host;
    this->help_message = other.help_message;
    this->headers = other.headers;
    this->_fd_socket = other._fd_socket;
    this->_pos_socket = other._pos_socket;
    this->_pos_file_response = other._pos_file_response;
    return *this;
}

struct sockaddr_in Response::getListen() const
{
    return (this->listen);
}

std::string Response::getHost() const
{
    return (this->host);
}

std::string Response::getRoot() const
{
    return (this->root);
}

std::string Response::getURI() const
{
    return (this->uri);
}

std::pair<int, std::string> Response::getRedirection() const
{
    return (this->redirection);
}

std::string Response::getIndex() const
{
    return (this->index);
}

bool Response::getAutoindex() const
{
    return (this->autoindex);
}

std::string Response::getAcceptMethod() const
{
    return (this->accept_method);
}

std::map<int, std::string> Response::getErrorPage() const
{
    return (this->error_page);
}

int Response::getErrorCode() const
{
    return (this->error_code);
}

bool Response::getCGI() const
{
    return (this->cgi);
}

std::string Response::getProtocol() const
{
    return (this->protocol);
}

std::string Response::getHelpMessage() const
{
    return (this->help_message);
}

std::map<std::string, std::string>  Response::getHeaders() const
{
    return (this->headers);
}

int Response::get_fd_socket() const
{
    return (this->_fd_socket);
}

int Response::get_pos_socket() const
{
    return (this->_pos_socket);
}

int Response::get_pos_file_response() const
{
    return (this->_pos_file_response);
}

struct stat Response::get_fileStat() const
{
    return (this->fileStat);
}

void Response::setListen(struct sockaddr_in listen)
{
    this->listen = listen;
}

void Response::setHost(std::string host)
{
    this->host = host;
}

void Response::setRoot(std::string root)
{
    this->root = root;
}

void Response::setURI(std::string uri)
{
    this->uri = uri;
}

void Response::setRedirection(std::pair<int, std::string> redirection)
{
    this->redirection = redirection;
}

void Response::setIndex(std::string index)
{
    this->index = index;
}

void Response::setAutoindex(bool autoindex)
{
    this->autoindex = autoindex;
}

void Response::setAcceptMethod(std::string accept_method)
{
    this->accept_method = accept_method;
}

void Response::setErrorPage(std::map<int, std::string>error_page)
{
    this->error_page = error_page;
}

void Response::setErrorCode(int error_code)
{
    this->error_code = error_code;
}

void Response::setCGI(bool cgi)
{
    this->cgi = cgi;
}

void Response::setProtocol(std::string protocol)
{
    this->protocol = protocol;
}

void Response::setHelpMessage(std::string help_message)
{
    this->help_message = help_message;
}

void Response::setHeaders(std::map<std::string, std::string> headers)
{
    this->headers = headers;
}

void    Response::err(int error, std::string root_error)
{
    error_code = error;
    root = root_error;
}

int Response::open_file(int pos_file_response)
{
    int fd_file = -1;

    _pos_file_response = pos_file_response;
    if (!redirection.second.empty() && !error_code)//si existe redireccion y no hay ningun error
        root = redirection.second;
    else
    {
        if (access(root.c_str(), F_OK) == -1)//si achivo no existe
            err(NOT_FOUND, ROOT_NOT_FOUND);
        else if (access(root.c_str(), R_OK) == -1)//si archivo no tiene permisos
            err(FORBIDEN, ROOT_FORBIDEN);
        if (stat(root.c_str(), &fileStat) == -1)
            err(NOT_FOUND, ROOT_NOT_FOUND);
        else if  ((fd_file = open(root.c_str(), O_RDONLY)) == -1)
            err(INTERNAL_SERVER_ERROR, ROOT_INTERNAL_SERVER_ERROR);
    }
    return (fd_file);
}
