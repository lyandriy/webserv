#include "Parser.hpp"

int main(int argc, char **argv)
{
    std::vector<Server*> server;

    try
    {
        if (argc == 2)
        {
            Parser  parser(argv[1]);
            server = parser.conf_file();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}