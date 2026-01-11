
#include "Server.hpp"

void chck_numbers_only(const char* arg)
{
    for(int i =0, arg[i], i++)
    {
        if(std::isdigit(arg[i]) == false)
            throw ServerInvalidInput();
    }
}

