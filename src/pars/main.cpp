#include "Parser.hpp"

int main(int argc, char **argv)
{
    try
    {
        if (argc == 2)
        {
            Parser  parser(argv[1]);
            parser.conf_file();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}