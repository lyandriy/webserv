#pragma once
#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

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
#include <algorithm>
#include <fstream>
#include <sstream>

# include "Request.hpp"
#include "Response.hpp"
#include "Parser.hpp"
# include "Error.hpp"
#include "httpMethods.hpp"
#include "Server.hpp"
#include "Location.hpp"

#endif