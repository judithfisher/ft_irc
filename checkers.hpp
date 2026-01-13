#pragma once

#include "Server.hpp"

// void chck_port_range(const char *arg);
// void chck_arguments_amount(int argc);
// void chck_numbers_only(const char *arg);
void check(int argc, char **argv); //Can't use const char** ! Not safe, pointer problem