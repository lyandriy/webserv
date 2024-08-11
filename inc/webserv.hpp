#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>

#define PORT    8080
#define IP      0x7f000001 //127.0.0.1 en unsigned long con la ordenación de bytes
#define BACKLOG 128