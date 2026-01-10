/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 18:12:48 by jfischer         ###   ########.fr       */
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
		// this->clients = other.clients;
	}
	return (*this);
}

Server::~Server()
{
}

int Server::getServerfd()
{
	return (this->server_fd);
}

void Server::setport(int port)
{
	this->port = port;
}

void Server::setpassword(std::string password)
{
	this->password = password;
}

// AF_INET (Address Family Internet) = IPv4
// Sock_Stream = reliable, ordered, connection-based byte stream = TCP
// htons = host to network short, 16 bits (port 6667-> 0x1A0B) 00001A0B -> 0B1A
void Server::InitServerSocket()
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;		
	addr.sin_port = htons(this->port);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol
	if (this->server_fd == -1)
	{
		std::cerr << "Socket creation failed" << std::endl;
		return;
	}
	
	if (bind(this->server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		return;
	}
	
	if (listen(this->server_fd, 0) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		return;
	}
}