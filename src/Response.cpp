# include "../inc/Webserver.hpp"

Response::Response(){}

Response::~Response()
{}

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
        else if (error_code == GATEWAY_TIMEOUT)
            this->root = ROOT_HTTP_GATEWAY_TIMEOUT;
        uri.clear();
    }
}

Response::Response(const Location &location, Request &request)
{
    this->root = location.getRoot();
    this->index = location.getIndex();
    this->redirection = location.getRedirection();
    this->uri = request.get_uri();
    if (!redirection.empty())
        uri = redirection;
    this->root_origin = root;
    this->error_page = location.getErrorPage();
    this->cgi = location.getCGI();
    this->autoindex = location.getAutoindex();
    this->listen.sin_family = AF_INET;
    this->listen.sin_port = htons(request.get_port());
    this->listen.sin_addr.s_addr = INADDR_ANY;
    this->accept_method = request.get_method();
    this->error_code = request.get_error_code();
    
    this->protocol = request.get_protocol();
    this->host = request.get_host();
    this->help_message = request.get_help_message();
    this->headers = request.get_headers();
    this->_pos_file_response = -1;
    this->_pos_socket = request.get_pos_socket();
    this->params = request.get_params();
    this->body = request.get_body();
    if (this->headers["Connection"].empty() || this->error_code != 200)
        this->connection_val = "close";
    else
        this->connection_val = this->headers["Connection"];
    if (this->error_code != 200)
        error_response();
    total_bytes_read = 0;
    send_size = 0;
    fd_pipe[0] = -1;
    fd_pipe[1] = -1;
    pipeRes = false;
    valread = -1;
    fileStat.st_size = 0;
    cgi_state = 0;
    upload_files = request.getUploadFiles();
    multipart = request.getMultipart();
}

Response::Response(const Server &server, Request &request)
{
    this->root = server.getRoot();
    this->index = server.getIndex();
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
    if (this->headers["Connection"].empty() || this->error_code != 200)
        this->connection_val = "close";
    else
        this->connection_val = this->headers["Connection"];
    if (this->error_code != 200)
        error_response();
    total_bytes_read = 0;
    send_size = 0;
    fd_pipe[0] = -1;
    fd_pipe[1] = -1;
    pipeRes = false;
    valread = -1;
    fileStat.st_size = 0;
    cgi_state = 0;
    upload_files = request.getUploadFiles();
    multipart = request.getMultipart();
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
    this->root_origin = other.root_origin;
    this->string_buffer = other.string_buffer;
    this->cgi_state = other.cgi_state;
    this->fd_pipe[0] = other.fd_pipe[0];
    this->fd_pipe[1] = other.fd_pipe[1];
    this->pipeRes = other.pipeRes;
    this->valread = other.valread;
    this->upload_files = other.upload_files; 
    this->multipart = other.multipart;
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

int Response::getFDread() const
{
    return (this->fd_pipe[0]);
}

int Response::getFDwrite() const
{
    return (this->fd_pipe[1]);
}

bool    Response::getPipeRes() const
{
    return (this->pipeRes);
}

int Response::getValread() const
{
    return (this->valread);
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

void    Response::setFDpipe(int fdw, int fdr)
{
    this->fd_pipe[0] = fdr;
    this->fd_pipe[1] = fdw;
}

void    Response::setPipeRes(bool piperes)
{
    this->pipeRes = piperes;
}

void    Response::setValread(int valread)
{
    this->valread = valread;
}

void    Response::closeFD()
{
    close(this->fd_pipe[0]);
    close(this->fd_pipe[1]);
}

void    Response::remove_sent_data(ssize_t send_size)
{
    this->string_buffer.erase(0, send_size);
}

void    Response::set_fileStatSize(int size)
{
    this->fileStat.st_size = size;
}

int    Response::open_error_file()
{
    int fd = -1;

    if (!error_page[error_code].empty())
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
        root = "serverHTML/error/" + oss.str() + ".html";
        stat(root.c_str(), &fileStat);
        fd = open(root.c_str(), O_RDONLY);
    }
    return (fd);
}

int Response::get_fd(std::string root)
{
    int fd_file = -1;
    
    if (stat(root.c_str(), &fileStat) == -1)
    {
        if (!(accept_method == "POST" && errno == ENOENT))
            error_code = NOT_FOUND;
    }
    else if (S_ISREG(fileStat.st_mode))
    {
        if (access(root.c_str(), R_OK) == -1)
            error_code = FORBIDEN;
        else if (cgi && error_code == 200 && root.size() > 3 
                && (root.substr(root.size() - 3) == ".py" || root.substr(root.size() - 4) == ".php" || root.substr(root.size() - 3) == ".pl"))
            cgi_state = 1;
        else if ((fd_file = open(root.c_str(), O_RDONLY)) == -1 || fcntl(fd_file, F_SETFD, O_CLOEXEC) == -1 || fcntl(fd_file, F_SETFL, O_NONBLOCK) == -1)
            error_code = INTERNAL_SERVER_ERROR;
    }
    else if (S_ISDIR(fileStat.st_mode))
    {
        join_with_uri(root, index);
        if ((fd_file = get_fd(root)) == -1 && autoindex)
            fd_file = make_autoindex_file();
    }
    return (fd_file);
}

void    Response::join_with_uri(std::string &root, std::string &uri)
{
    if (root[root.size() - 1] == '/' && uri[0] == '/')
        root.erase(root.size() - 1);
    else if (root[root.size() - 1] != '/' && uri[0] != '/')
        root += '/';
    root += uri;
    if (root[root.size() - 1] == '/')
        root.erase(root.size() - 1);
}

int Response::make_autoindex_file()
{
    std::string html_text;
    struct dirent *file_list;

    root = root_origin;
    join_with_uri(root, uri);
    DIR *dir = opendir(root.c_str());
    if (dir != NULL)
    {
        if (uri.rfind("/") != uri.size() - 1)
            uri += "/";
        if (pipe(fd_pipe) != -1)
        {
            html_text = HTML_AUTOINDEX_BEGIN;
            while ((file_list = readdir(dir)) != NULL)
                html_text += "      <li><a href=\"" + uri + file_list->d_name + "\">" + file_list->d_name + "</a></li>\n";
            html_text += HTML_AUTOINDEX_END;
            if (write(fd_pipe[1], html_text.c_str(), html_text.size()) != -1)
            {
                closedir(dir);
                error_code = OK;
                fileStat.st_size = html_text.size();
                pipeRes = true;
                return (fd_pipe[0]);
            }
        }
    }
    error_code = INTERNAL_SERVER_ERROR;
    return (-1);
}

int Response::open_file(int pos_file_response)
{
    int fd;
    _pos_file_response = pos_file_response;
    root_origin = root;
    join_with_uri(root, uri);
    fd = get_fd(root);
    if (fd == -1 && cgi_state == 0 && accept_method != "POST")
        fd = open_error_file();
    std::cout << root << std::endl;
    return (fd);
}

int Response::postIsExec()
{
    if (root.size() > 3 && (root.substr(root.size() - 3) == ".py" || root.substr(root.size() - 4) == ".php" || root.substr(root.size() - 3) == ".pl"))
    {
        cgi_state = 1;
        return (1);
    }
    return (0);
}

int Response::makePost()
{
    int fd = -1;
    struct stat fileStat_;
    std::string str(body.begin(), body.end());
    if (stat(root.c_str(), &fileStat_) == -1)
    {
        std::ofstream file(root.c_str(), std::ios::app);
        if (file.is_open())
            file << str;
        else
        {
            error_code = FORBIDEN;
            fd = open_error_file();
        }
        if (fd == -1)
            fd = get_fd("serverHTML/postResponse.html");
        return (fd);
    }
    else if (S_ISDIR(fileStat_.st_mode))
        join_with_uri(root, index);
    if (stat(root.c_str(), &fileStat_) == -1)
    {
        error_code = FORBIDEN;
        fd = open_error_file();
        return (fd);
    }
    std::ofstream file(root.c_str(), std::ios::app);
    if (file.is_open())
        file << str;
    else
    {
        error_code = FORBIDEN;
        fd = open_error_file();
    }
    if (fd == -1)
        fd = get_fd("serverHTML/postResponse.html");
    return (fd);
}

int Response::makeUpload()
{
    int fd = -1;
    std::map <std::string, std::string>::iterator it;
    std::string filename;
    std::string filename_;
    struct stat fileStat_;

    if (upload_files.empty())
    {
        error_code = NOT_FOUND;
        fd = open_error_file();
        return (fd);
    }
    for (it = upload_files.begin(); it != upload_files.end(); ++it)
    {
        join_with_uri(root, uri);
        filename = root;
        if (stat(filename.c_str(), &fileStat_) == -1)
        {
            error_code = FORBIDEN;
            fd = open_error_file();
            return (fd);
        }
        else if (S_ISDIR(fileStat_.st_mode))
        {
            filename_ = it->first;
            join_with_uri(filename, filename_);
        }
        std::ofstream file(filename.c_str(), std::ios::app);
        if (file.is_open())
            file << it->second;
        else
        {
            error_code = FORBIDEN;
            fd = open_error_file();
            break ;
        }
    }
    if (fd == -1)
        fd = get_fd("serverHTML/postResponse.html");
    return (fd);
}
