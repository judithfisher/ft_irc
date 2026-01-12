
#include "Server.hpp"

void chck_arguments_amount(int argc)
{
    if (argc != 3)
        throw ServerInvalidInput();
}

void chck_numbers_only(const char* arg)
{
    for(int i =0; arg[i]; i++)
    {
        if(std::isdigit(arg[i]) == false)
            throw ServerInvalidInput();
    }
}

void check(int argc, char *argv[])
{
    check_arguments_amount(argc);
    check_numbers_only(argv[1]);
}
