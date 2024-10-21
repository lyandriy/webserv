# include "../inc/Webserver.hpp"

Response::Response(){}

Response::~Response(){}

Response::Response(const Response &other){
    *this = other;
}

Response::Response(const Location &location, Request &request){
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
    if (request.get_headers().find("Connection") == request.get_headers().end())
        this->connection_val = "keep-alive";
    else
        this->connection_val = request.get_headers()["Connection"];
    total_bytes_read = 0;
}

Response::Response(const Server &server, Request &request){
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
    if (request.get_headers().find("Connection") == request.get_headers().end())
        this->connection_val = "keep-alive";
    else
        this->connection_val = request.get_headers()["Connection"];
    total_bytes_read = 0;
}

Response::Response(Request &request)
{
    this->listen.sin_family = AF_INET;
    this->listen.sin_port = htons(request.get_port());
    this->listen.sin_addr.s_addr = INADDR_ANY;
    this->help_message = request.get_help_message();
    this->error_code = request.get_error_code();
    this->uri = request.get_uri();
    this->protocol = request.get_protocol();
    this->host = request.get_host();
    this->help_message = request.get_help_message();
    this->headers = request.get_headers();
    this->_pos_file_response = -1;
    if (request.get_error_code() == BAD_REQUEST)
        this->root = ROOT_BAD_REQUEST;
    else if (request.get_error_code() == FORBIDEN)
        this->root = ROOT_FORBIDEN;
    else if (request.get_error_code() == NOT_FOUND)
        this->root = ROOT_NOT_FOUND;
    else if (request.get_error_code() == METHOD_NOT_ALLOWED)
        this->root = ROOT_METHOD_NOT_ALLOWED;
    else if (request.get_error_code() == REQUEST_TIMEOUT)
        this->root = ROOT_REQUEST_TIMEOUT;
    else if (request.get_error_code() == CONTENT_TOO_LARGE)
        this->root = ROOT_CONTENT_TOO_LARGE;
    else if (request.get_error_code() == URI_TOO_LONG)
        this->root = ROOT_URI_TOO_LONG;
    else if (request.get_error_code() == INTERNAL_SERVER_ERROR)
        this->root = ROOT_INTERNAL_SERVER_ERROR;
    else if (request.get_error_code() == SERVICE_UNAVAIBLE)
        this->root = ROOT_SERVICE_UNAVAIBLE;
    else if (request.get_error_code() == HTTP_VERSION_NOT_SUPPORTED)
        this->root = ROOT_HTTP_VERSION_NOT_SUPPORTED;
    this->_pos_file_response = -1;
    this->connection_val = "close";
    total_bytes_read = 0;
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
    this->fileStat = other.fileStat;
    this->connection_val = other.connection_val;
    this->total_bytes_read = other.total_bytes_read;
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

std::string Response::getRedirection() const
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

std::string Response::getConnectionVal() const
{
    return (this->connection_val);
}

size_t  Response::getBytesRead() const
{
    return (this->total_bytes_read);
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

void Response::setRedirection(std::string redirection)
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

void Response::setConnectionVal(std::string connection_val)
{
    this->connection_val = connection_val;
}

void    Response::setBytesRead(size_t bytes_read)
{
    this->total_bytes_read += bytes_read;
}

void    Response::err(int error, std::string root_error)
{
    error_code = error;
    root = root_error;
}

int    Response::internServerError()
{
    int fd_file;

    if ((fd_file = open(ROOT_INTERNAL_SERVER_ERROR, O_CREAT)) == -1)
        err(INTERNAL_SERVER_ERROR, ROOT_INTERNAL_SERVER_ERROR);
    if (write(fd_file, "Error 500. Internal server error", 32) == -1)
        err(INTERNAL_SERVER_ERROR, ROOT_INTERNAL_SERVER_ERROR);
    error_code = INTERNAL_SERVER_ERROR;
    return (fd_file);
}

int Response::make_autoindex_file()
{
    int fd_file = -1;
    std::string html_text;
    struct dirent *file_list;
    root.erase((root.size() - index.size()), root.size());
    DIR *dir = opendir(root.c_str());
    if (dir == NULL)
        return (internServerError());
    else
    {
        if ((fd_file = open(AUTOINDEX_FILE, O_CREAT)) == -1)
            err(INTERNAL_SERVER_ERROR, ROOT_INTERNAL_SERVER_ERROR);
        while ((file_list = readdir(dir)) != NULL)
        {   
            if (std::string(file_list->d_name) != "." && std::string(file_list->d_name) != "..")
                html_text = html_text + "<li><a href=\"" + root + file_list->d_name + "\">" + file_list->d_name + "</a></li>\n";
        }
        if (write(fd_file, html_text.c_str(), html_text.size()) == -1)
            err(INTERNAL_SERVER_ERROR, ROOT_INTERNAL_SERVER_ERROR);
        closedir(dir);
        close(fd_file);
        return (get_fd(root));
    }
    return (internServerError());
}

int Response::get_fd(std::string root)
{
    int fd_file = -1;

    if (stat(root.c_str(), &fileStat) == -1)
        err(NOT_FOUND, ROOT_NOT_FOUND);
    if (access(root.c_str(), F_OK) == -1)//si achivo no existe
        err(NOT_FOUND, ROOT_NOT_FOUND);
    else if (access(root.c_str(), R_OK) == -1)//si archivo no tiene permisos
        err(FORBIDEN, ROOT_FORBIDEN);
    if ((fd_file = open(root.c_str(), O_RDONLY)) == -1)
    {
        err(INTERNAL_SERVER_ERROR, ROOT_INTERNAL_SERVER_ERROR);
        return (internServerError());
    }
    return (fd_file);
}

int Response::open_file(int pos_file_response)
{
    _pos_file_response = pos_file_response;
    if (!redirection.empty() && !error_code)//si existe redireccion y no hay ningun error
        root = redirection;
    root += uri;
    std::cout << "\033[33m" << " root " << root <<  "\033[0m" << std::endl;
    if (stat(root.c_str(), &fileStat) == -1)
    {
        err(NOT_FOUND, ROOT_NOT_FOUND);
        return (get_fd(root));
    }
    if (S_ISREG(fileStat.st_mode))//si la ruta es un archivo
        return (get_fd(root));
    else if (S_ISDIR(fileStat.st_mode))//si la ruta es un directorio
    {
        if (root.rfind("/") != root.size() - 1)
            root += "/";
        root += index;
        std::cout << "\033[33m" << " index " << index <<  "\033[0m" << std::endl;
        std::cout << "\033[33m" << " root " << root <<  "\033[0m" << std::endl;
        if (stat(root.c_str(), &fileStat) == -1)
            return (make_autoindex_file());
        else
            return (get_fd(root));
    }
    return (internServerError());
}

//*********************** PRINT RESPONSE ************************//

void Response::print_full_info() {
    std::cout << "-------- Response Info --------" << std::endl;

    // Información básica
    struct sockaddr_in listen_info = getListen();
    std::cout << "Listen (Port): " << ntohs(listen_info.sin_port) << std::endl;
    std::cout << "Host (Server Name): " << getHost() << std::endl;
    std::cout << "Root Directory: " << getRoot() << std::endl;
    std::cout << "URI: " << getURI() << std::endl;

    // Redirección
    std::cout << "Redirection URI: " << getRedirection() << std::endl;

    // Otras configuraciones
    std::cout << "Index File: " << getIndex() << std::endl;
    std::cout << "Autoindex: " << (getAutoindex() ? "Enabled" : "Disabled") << std::endl;
    std::cout << "Accepted Method: " << getAcceptMethod() << std::endl;

    // Páginas de error
    std::cout << "Error Pages: " << std::endl;
    std::map<int, std::string> error_pages = getErrorPage();
    std::map<int, std::string>::const_iterator it;
    for (it = error_pages.begin(); it != error_pages.end(); ++it) {
        std::cout << "  Error " << it->first << ": " << it->second << std::endl;
    }

    // Estado del CGI
    std::cout << "CGI Enabled: " << (getCGI() ? "true" : "false") << std::endl;

    // Protocolo y mensajes
    std::cout << "Protocol: " << getProtocol() << std::endl;
    std::cout << "Help Message: " << getHelpMessage() << std::endl;

    // Headers
    std::cout << "Headers: " << std::endl;
    std::map<std::string, std::string> headers = getHeaders();
    std::map<std::string, std::string>::const_iterator headers_it;
    for (headers_it = headers.begin(); headers_it != headers.end(); ++headers_it) {
        std::cout << "  " << headers_it->first << ": " << headers_it->second << std::endl;
    }

    // Información del socket
    std::cout << "File Descriptor (Socket FD): " << get_fd_socket() << std::endl;
    std::cout << "Socket Position: " << get_pos_socket() << std::endl;
    std::cout << "File Response Position: " << get_pos_file_response() << std::endl;

    // Información del archivo
    struct stat file_stat = get_fileStat();
    std::cout << "File Size: " << file_stat.st_size << std::endl;
    std::cout << "Last Modified: " << ctime(&file_stat.st_mtime); // Convierte time_t a formato legible

    // Valor de conexión
    std::cout << "Connection Value: " << getConnectionVal() << std::endl;

    std::cout << "--------------------------------" << std::endl;
}
