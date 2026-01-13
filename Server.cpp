/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/13 18:48:50 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include "Server.hpp"
#include <cerrno> 

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

// AF_INET (Address Family Internet) = IPv4
// Sock_Stream = reliable, ordered, connection-based byte stream = TCP
// htons = host to network short, 16 bits (port 6667-> 0x1A0B) 00001A0B -> 0B1A
void Server::InitServerSocket()
{
	sockaddr_in addr;
	struct pollfd NewPoll;						// we don't know what this is for yet
	
	addr.sin_family = AF_INET;		
	addr.sin_port = htons(this->port);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol
	if (this->server_fd == -1)
	{
		std::cerr << "Socket creation failed" << std::endl;
		return;
	}

	int en = 1;
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) < 0)
	{
		std::cerr << "Setsockopt failed" << std::endl;
		return;
	}

	if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)	// we don't know this one yet
	{
		std::cerr << "Failed to set non-blocking mode" << std::endl;
		return;
	}
	
	if (bind(this->server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		return;
	}
	
	if (listen(this->server_fd, SOMAXCONN) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		return;
	}

	NewPoll.fd = this->server_fd;
	NewPoll.events = POLLIN; 					// set the event to POLLIN to read data
	NewPoll.revents = 0;						// set the revents to 0
	fds.push_back(NewPoll);						// add the server_fd to the pollfd vector
}

void Server::AcceptClients()
{
	int client_fd;
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
	{
		if (errno == EINTR && Server::SignalReceived)
			return ;
	// 	std::cerr << "Error accepting client connection" << std::endl;
		return ;
	}

	clients.push_back(Client(client_fd));
	std::cout << "new client connected" << std::endl;
}

void Server::ClearClients()
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        int fd = clients[i].getFd();
        if (fd >= 0)
            close(fd);
    }
    clients.clear();
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