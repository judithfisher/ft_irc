
#include "checkers.hpp"

static void chck_port_range(const char *arg)
{
    int nbr = std::atoi(arg);
    std::cout << nbr << std::endl;
    if (nbr < 1024 || nbr > 65535)
        throw Server::InvalidRange();
}

static void chck_arguments_amount(int argc)
{
    if (argc != 3)
        throw Server::InvalidInput();
}

static void chck_numbers_only(const char *arg)
{
    for(int i =0; arg[i]; i++)
    {
        if(std::isdigit(arg[i]) == false)
            throw Server::InvalidInput();
    }
}

void check(int argc, char **argv)
{
    chck_arguments_amount(argc);
    chck_numbers_only(argv[1]);
    chck_port_range(argv[1]);
}
