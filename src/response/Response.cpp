# include "../inc/Webserver.hpp"

Response::Response(){}

Response::~Response(){}

Response::Response(const Response &other)
{
    *this = other;
}

void    Response::error_response()
{
    this->connection_val = CLOSE;
    if (!error_page[error_code].empty())
        uri = error_page[error_code];
    else
    {
        if (error_code == BAD_REQUEST)
            this->root = ROOT_BAD_REQUEST;
        else if (error_code == FORBIDEN)
            this->root = ROOT_FORBIDEN;
        else if (error_code == NOT_FOUND)
            this->root = ROOT_NOT_FOUND;
        else if (error_code == METHOD_NOT_ALLOWED)
            this->root = ROOT_METHOD_NOT_ALLOWED;
        else if (error_code == REQUEST_TIMEOUT)
            this->root = ROOT_REQUEST_TIMEOUT;
        else if (error_code == CONTENT_TOO_LARGE)
            this->root = ROOT_CONTENT_TOO_LARGE;
        else if (error_code == URI_TOO_LONG)
            this->root = ROOT_URI_TOO_LONG;
        else if (error_code == INTERNAL_SERVER_ERROR)
            this->root = ROOT_INTERNAL_SERVER_ERROR;
        else if (error_code == SERVICE_UNAVAIBLE)
            this->root = ROOT_SERVICE_UNAVAIBLE;
        else if (error_code == HTTP_VERSION_NOT_SUPPORTED)
            this->root = ROOT_HTTP_VERSION_NOT_SUPPORTED;
        uri.clear();
    }
}

Response::Response(const Location &location, Request &request)
{
    this->root = location.getRoot();
    this->index = location.getIndex();
    this->redirection = location.getRedirection();
    if (!redirection.empty())
        root = redirection;
    this->root_origin = root;
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
    this->_pos_socket = request.get_pos_socket();
    this->params = request.get_params();
    this->body = request.get_body();
    if (request.get_headers().find("Connection") == request.get_headers().end())
        this->connection_val = "keep-alive";
    else
        this->connection_val = request.get_headers()["Connection"];
    if (this->error_code != 200)
        error_response();
    total_bytes_read = 0;
    send_size = 0;
}

Response::Response(const Server &server, Request &request){
    this->root = server.getRoot();
    this->index = server.getIndex();
    this->redirection = server.getRedirection();
    if (!redirection.empty())
        root = redirection;
    this->root_origin = root;
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
    this->_pos_socket = request.get_pos_socket();
    this->params = request.get_params();
    this->body = request.get_body();
    if (this->headers["Connection"].empty())
    {
        this->connection_val = "close";
        std::cout << "\033[33m" << " MAIN 1" << this->connection_val <<  "\033[0m" << std::endl;
    }
    else
    {
        this->connection_val = this->headers["Connection"];
        std::cout << "\033[33m" << " MAIN " << this->connection_val <<  "\033[0m" << std::endl;
    }
    if (this->error_code != 200)
        error_response();
    total_bytes_read = 0;
    send_size = 0;
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
    this->send_size = other.send_size;
    this->params = other.params;
    this->body = other.body;
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

std::string Response::getStringBuffer() const
{
    return (this->string_buffer);
}

int Response::getSendSize() const
{
    return (static_cast<int>(this->send_size));
}

std::map<std::string, std::string>  Response::getParams() const
{
    return (this->params);
}

std::vector<char>   Response::getBody() const
{
    return (this->body);
}

int Response::getCGIState() const
{
    return (this->cgi_state);
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

void    Response::setStringBuffer(std::string buffer)
{
    this->string_buffer += buffer;
}

void    Response::setSendSize(ssize_t size)
{
    this->send_size += size;
}

void Response::setParams(std::map<std::string, std::string> params)
{
    this->params = params;
}

void Response::setBody(std::vector<char> body)
{
    this->body = body;
}

void    Response::setCGIState(int cgi_state)
{
    this->cgi_state = cgi_state;
}

void    Response::remove_sent_data(ssize_t send_size)
{
    this->string_buffer.erase(0, send_size);
}

int    Response::open_error_file()
{
    int fd = -1;

    if (!error_page[error_code].empty())//si el archivo de error esta configurado
    {
        root = root_origin;
        join_with_uri(root, error_page[error_code]);
        stat(root.c_str(), &fileStat);
        fd = open(root.c_str(), O_RDONLY);
    }
    if (fd == -1)
    {
        std::ostringstream oss;
        oss << error_code;
        root = "html/error/" + oss.str() + ".html";//si el archivo de error no esta configurado se retorna el de server
        stat(root.c_str(), &fileStat);
        fd = open(root.c_str(), O_RDONLY);
    }
    return (fd);
}

int Response::get_fd(std::string root)
{
    int fd_file = -1;
    
    if (stat(root.c_str(), &fileStat) == -1)
        error_code = NOT_FOUND;
    if (S_ISREG(fileStat.st_mode))//si la ruta es un archivo
    {
        if (access(root.c_str(), R_OK) == -1)//si archivo no tiene permisos
            error_code = FORBIDEN;
        else if (cgi && error_code == 200)
            cgi_state = 1;//
        else if ((fd_file = open(root.c_str(), O_RDONLY)) == -1)
            error_code = INTERNAL_SERVER_ERROR;
    }
    return (fd_file);
}

void    Response::join_with_uri(std::string &root, std::string &uri)
{
    if (root[root.size() - 1] == '/' && uri[0] == '/')
        root.erase(root.size() - 1);
    else if (root[root.size() - 1] != '/' && uri[0] != '/')
        root += '/';
    root += uri;//unimos dos strings
    if (root[root.size() - 1] == '/')//si  tiene /, eliminamos
        root.erase(root.size() - 1);
}

int Response::make_autoindex_file()
{
    std::string html_text;
    int fd[2];
    struct dirent *file_list;

    root = root_origin;
    join_with_uri(root, uri);//recupero la ruta sin el autoindex
    DIR *dir = opendir(root.c_str());//abro el dir
    if (dir != NULL)//reviso si se puede habrir
    {
        if (uri.rfind("/") != uri.size() - 1)//compribar que la uri termina con /
            uri += "/";
        if (pipe(fd) != -1)//abrimos pipe
        {
            html_text = HTML_AUTOINDEX_BEGIN;//metemos el texto de inicio
            while ((file_list = readdir(dir)) != NULL)
                html_text += "      <li><a href=\"" + uri + file_list->d_name + "\">" + file_list->d_name + "</a></li>\n";
            html_text += HTML_AUTOINDEX_END;//metemos el texto de final
            if (write(fd[1], html_text.c_str(), html_text.size()) != -1)//escribimos en pipe
            {
                closedir(dir);//cerramos el directorio
                close(fd[1]);//cerramos el pipe de escritura
                error_code = OK;
                fileStat.st_size = html_text.size();//poner una fcntl
                return (fd[0]);
            }
        }
    }
    error_code = INTERNAL_SERVER_ERROR;
    return (-1);
}

int Response::open_file(int pos_file_response)
{
    int fd;
    _pos_file_response = pos_file_response;//posicion del fd en pollfd del archivo que se v a enviar al cliente
    root_origin = root;//copiamos rota original
    join_with_uri(root, uri);
    fd = get_fd(root);//stat + abrimos ruta + uri
    if (S_ISDIR(fileStat.st_mode))//si la ruta es un directorio
    {
        join_with_uri(root, index);//root + index
        if ((fd = get_fd(root)) == -1 && autoindex)//probamos root + index, si no existe y autoindex es on
            fd = make_autoindex_file();//hacemos el archivo de autoindex
    }
    if (fd == -1 && cgi_state == 0)
        fd = open_error_file();//funcion que abre archivo de error
    return (fd);
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
