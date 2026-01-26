#include "Server.hpp"
#include <cstring>
#include <cerrno>

void log_error(const std::string &where)
{
	std::cerr << "[ERROR] " << where
		<< " | errno=" << errno
		<< " (" << std::strerror(errno) << ")"
		<< std::endl;
}

// RFC-compliant line ending for IRC replies (CRLF).
void Server::sendLine(int fd, const std::string &msg)
{
	std::string out = msg + "\r\n";
	send(fd, out.c_str(), out.size(), 0);
}
