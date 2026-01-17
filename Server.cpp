/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/17 20:59:39 by jfischer         ###   ########.fr       */
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

// fcntl = file control, manipulate file descriptor
// O_NONBLOCK = non-blocking mode, I/O operations on the socket will be non-blocking
// accept(), rec(), send return immediately with data or error instead of blocking


void Server::InitServerSocket()
{
	sockaddr_in addr;							// from library, structure to hold server address information	
	struct pollfd NewPoll;						// from library, structure for poll monitoring, tells poll which fds to monitor and what events to look for
	
	addr.sin_family = AF_INET;					// IPv4
	addr.sin_port = htons(this->port);			// port number in network byte order
	addr.sin_addr.s_addr = INADDR_ANY;			// accept connections from any IP address
	
	// this->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // IPv4, TCP, explicit protocol
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol (kernel chooses TCP for SOCK_STREAM)
	if (this->server_fd == -1)
	{
		std::cerr << "Socket creation failed" << std::endl;
		return;
	}

	int en = 1;
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) < 0) //Allows reusing the same port immediately after program restart
	{
		std::cerr << "Setsockopt failed" << std::endl;
		return;
	}

	if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)	// all I/O operations on the socket will be non-blocking ( read accept do not freez server when no data is available)
	{
		std::cerr << "Failed to set non-blocking mode" << std::endl;
		close (this->server_fd);
		return;
	}
	
	if (bind(this->server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) //attach socket to an IP address and port. (socket will listen on this specific address:port)
	{
		std::cerr << "Bind failed" << std::endl;
		return;
	}
	
	if (listen(this->server_fd, SOMAXCONN) < 0) //put socket into listening  mode (allow clients to connect)
	{
		std::cerr << "Listen failed" << std::endl;
		close (this->server_fd);
		return;
	}

	NewPoll.fd = this->server_fd;
	NewPoll.events = POLLIN; 					// set the event to POLLIN to read data
	NewPoll.revents = 0;						// set the revents to 0
	fds.push_back(NewPoll);						// add the server_fd to the pollfd vector

	std::cout << "Server listening on port " << this->server_fd << std::endl;
}

// poll puts your process to sleep and wakes it when at least 1 fd becomes usable for the kind of I/O signal you care about (fds.events[i]) or when time runs out
// poll return value = total number of fds that have been selected (fds for which the revents member is non-zero), 0 indicates time-out (no fds selected), -1 some error or CRTL+C
void Server::RunServer()
{
	while (SignalReceived == false)
	{
		int poll_count = poll(fds.data(), fds.size(), -1); // -1 = no time-out
		if (poll_count == -1)
		{
			std::cerr << "Poll failed " << std::endl;
			break ;  
		}

		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == this->server_fd)	// is it server socket? new client connecting
					AcceptClients();
				else								// client socket
					ReceiveData(fds[i].fd);
			}

			if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))	// POLLHUP = hang up (client disconnected), POLLNVAL = invalid fd, POLLERR = error on the existing fd
			{
				std::cerr << "Error on fd: " << fds[i].fd << std::endl;
				RemoveClient(fds[i].fd);
			}
		}
	}
}

void Server::AcceptClients()
{
	int client_fd;
	struct pollfd NewPoll;
	// sockaddr_in client_addr;									// not needed if client address is not used
	// socklen_t client_len = sizeof(client_addr);				// not needed if client address is not used

	while (SignalReceived == false)
	{
		// client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
		client_fd = accept(server_fd, NULL, NULL);				// if client address is not needed, NULL can be passed as 2nd and 3rd argument
		if (client_fd < 0)
			break ;
		fcntl(client_fd, F_SETFL, O_NONBLOCK);					// set client socket to non-blocking mode
		NewPoll.fd = client_fd;
		NewPoll.events = POLLIN;
		NewPoll.revents = 0;
		fds.push_back(NewPoll);
		clients.push_back(Client(client_fd));					// not sure if vector of clients is needed
		std::cout << "New client connected, client_fd: " << clients.back().getFd() <<  " total clients: " << clients.size() << std::endl;
	}
}

/*
─────────────────────────────────────────────────┐
│              CLIENT BUFFER (string)            │
├────────────────────────────────────────────────┤
│                                                │
│  AppendToBuffer("NI")                          │
│  ↓                                             │
│  buffer = "NI"                                 │
│                                                │
│  AppendToBuffer("CK alice\r\n")                │
│  ↓                                             │
│  buffer = "NICK alice\r\n"                     │
│                                                │
│  ExtractCompleteCommands()                     │
│  ↓                                             │
│  Found: "NICK alice"                           │
│  buffer = "" (cleared)                         │
│  Returns: ["NICK alice"]                       │
│                                                │
└────────────────────────────────────────────────┘
*/

void Server::ReceiveData(int client_fd)
{
	char buffer[MAX_BUFFER_SIZE + 1]; // +1 for null-terminator
	// buffer[0] = '\0';
	int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0)
	{
		if (bytes_received == 0)
			std::cout << "Client disconnected, client_fd: " << client_fd << std::endl;
		
		else
			std::cerr << "Recv failed for client_fd: " << client_fd << " Error: " << strerror(errno) << std::endl;
		RemoveClient(client_fd);
		return;
	}
	
	buffer[bytes_received] = '\0'; // Null-terminate the received data
	std::cout << "Receiving data from client_fd: " << client_fd << std::endl;

	std::string str_buffer(buffer, bytes_received);			// std::string(buffer, bytes_received) Constructor for string
	clients[client_fd].AppendToBuffer(str_buffer);
	
	std::vector<std::string> commands = clients[client_fd].ExtractCompleteCommands();
	for (size_t i = 0; i < commands.size(); i++)
		ProcessCommand(client_fd, commands[i]);		// implement command processing logic here
}

std::vector<std::string> split(const std::string& line)
{
    std::vector<std::string> tokens;
    std::istringstream istreami(line);
    std::string t;

    while (istreami >> t)
        tokens.push_back(t);

    return (tokens);
}



void Server::ProcessCommand(int client_fd, const std::string &line)
{
	std::vector<std::string> tokens = split(line);
	
	if (tokens.empty())
		return;
		
	const std::string &command = tokens[0];

	if (command == "QUIT")					// just a check
	{
		std::cout << "Client requested to quit, client_fd: " << client_fd << std::endl;
		RemoveClient(client_fd);
		return;
	}
	std::cout << "Received command from client_fd " << client_fd << ": " << command << std::endl; 	// just for testing
	
		// if PRIVMSG send message to target
		// if JOIN add client to channel
		// etc...
}

void Server::RemoveClient(int client_fd)
{
	close (client_fd);
    for (size_t i = 0; i < fds.size(); i++)
    {
        if (fds[i].fd == client_fd)
        {
            fds.erase(fds.begin() + i);
			std::cout << "Pollfd removed, client_fd: " << client_fd << " total fds: " << fds.size() << std::endl;
            break;  							// Found it, stop searching
        }
    }

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getFd() == client_fd)
		{
			clients.erase(clients.begin() + i);
			std::cout << "Client removed, client_fd: " << client_fd << " total clients: " << clients.size() << std::endl;
			break;
		}
	}
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