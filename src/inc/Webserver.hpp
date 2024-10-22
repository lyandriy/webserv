# pragma once
# ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# define MAX_CLIENTS 20
# define BUFFER_SIZE 145
# define BACKLOG 1024

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <cerrno>
# include <poll.h>
# include <map>
# include <vector>
# include <cstring>
# include <algorithm>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <fcntl.h>
# include <ctime>
# include <sys/stat.h>
# include <stdlib.h>

#include <iomanip>

# include "httpMethods.hpp"
# include "Location.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "SocketManager.hpp"
# include "Parser.hpp"
# include "Error.hpp"

# endif