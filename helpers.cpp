
#include "Server.hpp"
#include "Client.hpp"
#include <cstring>
#include <cerrno>

void log_error(const std::string& where)
{
    std::cerr << "[ERROR] " << where
              << " | errno=" << errno
              << " (" << std::strerror(errno) << ")"
              << std::endl;
}