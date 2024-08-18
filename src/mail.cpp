#include "webserv.hpp"

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        conf_file(argv[1]);
    }
}
