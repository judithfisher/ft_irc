/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judith <judith@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/24 18:05:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
}

Server::Server(const Server &other)
{
	*this = other;
}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->port = other.port;
		this->server_fd = other.server_fd;
	}
	return (*this);
}

Server::~Server()
{
}

void Server::SignalHandler(int signum)
{
	(void) signum;
	SignalReceived = true;
}

int Server::getServerfd()
{
	return (this->server_fd);
}

std::vector<Client> Server::getClientvector()
{
	return (clients);
}

void Server::setport(int port)
{
	this->port = port;
}

void Server::setpassword(std::string password)
{
	this->password = password;
}

int Server::findClientbyFd(int client_fd)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getFd() == client_fd)
			return static_cast<int>(i);
	}
	std::cerr << "Client not found, client_fd: " << client_fd << std::endl;
	return (-1);
}

const char *Server::InvalidInput::what() const throw()
{
	return (RED "Invalid input: " R BLU "Port argument can only consist digits 0-9.\n (Note: zero as first digit is also not allowed)" R);
}

const char *Server::InvalidArgsAmount::what() const throw()
{
	return (RED "Invalid arguments amount: "  R BLU "Usage: ./ft_irc <port> <password>" R);
}

const char *Server::InvalidRange::what()  const throw()
{
	return (RED "invalid port number: " R BLU "try between 1024 - 65535" R);
}

const char *Server::PassLengh::what() const throw()
{
	return (RED "Password should be between 3 to 16 characters" R);
}

const char *Server::PassAsciOnly::what() const throw()
{
	return (RED "Password can only consist ASCII characters" R);
}
