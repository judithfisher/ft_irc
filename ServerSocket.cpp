/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 18:05:00 by codex             #+#    #+#             */
/*   Updated: 2026/02/02 21:13:18 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Socket lifecycle and I/O loop
void Server::InitServerSocket()
{
	sockaddr_in addr;							// server address information
	struct pollfd NewPoll;						// pollfd entry for the listening socket

	addr.sin_family = AF_INET;					// IPv4
	addr.sin_port = htons(this->port);			// port number in network byte order
	addr.sin_addr.s_addr = INADDR_ANY;			// accept connections from any IP address

	this->server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol
	if (this->server_fd == -1)
	{
		std::cerr << "Socket creation failed" << std::endl;
		return;
	}

	// SO_REUSEADDR allows fast restart without waiting for TIME_WAIT sockets to expire.
	int en = 1;
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) < 0)
	{
		std::cerr << "Setsockopt failed" << std::endl;
		return;
	}

	// Non-blocking avoids the server freezing on accept/recv when no data is available.
	if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Failed to set non-blocking mode" << std::endl;
		close(this->server_fd);
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
		close(this->server_fd);
		return;
	}

	NewPoll.fd = this->server_fd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	fds.push_back(NewPoll);

	std::cout << "Server listening on fd: " << this->server_fd << std::endl;
}

// poll puts your process to sleep and wakes it when fds become readable or error.
void Server::RunServer()
{
	while (SignalReceived == false)
	{
		int poll_count = poll(fds.data(), fds.size(), -1); // -1 = no timeout
		if (poll_count == -1)
		{
			// poll can return -1 on shutdown (e.g. signal); exit loop to clean up.
			// std::cerr << "Poll failed " << std::endl;
			break;
		}

		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == this->server_fd)
				{
					AcceptClients();
				}
				else
				{
					ReceiveData(fds[i].fd);
				}
			}

			if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
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

	while (SignalReceived == false)
	{
		client_fd = accept(server_fd, NULL, NULL);
		if (client_fd < 0)
			break;
		fcntl(client_fd, F_SETFL, O_NONBLOCK);
		NewPoll.fd = client_fd;
		NewPoll.events = POLLIN;
		NewPoll.revents = 0;
		fds.push_back(NewPoll);
		clients.push_back(Client(client_fd));
		std::cout << "New client connected, client_fd: " << clients.back().getFd()
			<<  " total clients: " << clients.size() << std::endl;
	}
}

void Server::ReceiveData(int client_fd)
{
	char buffer[MAX_BUFFER_SIZE + 1]; // +1 for null-terminator
	int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0)
	{
		if (bytes_received == 0)
			std::cout << "Client disconnected, client_fd: " << client_fd << std::endl;
		else
			std::cerr << "Recv failed for client_fd: " << client_fd << std::endl;
		RemoveClient(client_fd);
		return;
	}

	buffer[bytes_received] = '\0';
	std::cout << "Receiving data from client_fd: " << client_fd << std::endl;

	int client_index = findClientbyFd(client_fd);
	if (client_index < 0)
		return;

	// clients[client_index].setBuffer(std::string(buffer, bytes_received));
	std::string buffer1 = std::string(buffer, bytes_received);
	clients[client_index].AppendToBuffer(buffer1);
	std::vector<std::string> cmds = clients[client_index].ExtractCompleteCommands();

	for (size_t i = 0; i < cmds.size(); i++)
	{	
		std::cout << "process raw command: " << cmds[i] << std::endl;
		ProcessCommand(client_fd, cmds[i]);
		std::cout << cmds[i] << std::endl;
	}
}

void Server::RemoveClient(int client_fd)
{
	int client_index = findClientbyFd(client_fd);
	std::string nick = clients[client_index].getNickname();
	std::string user = clients[client_index].getUsername();
	// Ensure channel membership is cleaned up (fds get reused by the OS).
	for (std::map<std::string, Channel>::iterator it = channels.begin();
		it != channels.end();)
	{
		/* //broadcast to everyone that client left + what he wrote 
		it->second.broadcast(":" + nick + "!" + user + "@host QUIT :Client Quit"); */
		
		it->second.removeUser(client_fd);
		it->second.removeOperator(client_fd);
		if (it->second.getUserCount() == 0)
		{
			std::cout << "Channel removed: " << it->first << std::endl;
			channels.erase(it++);
			continue;
		}
		else if (it->second.getOperatorsSize() < 1)/* MONDAY adding operator after deleting previous one upon quitting */
		{
			int new_opr_fd = it->second.getFirstUser();
			int new_cln_index = findClientbyFd(new_opr_fd);
			it->second.addOperator(new_opr_fd);
			it->second.broadcast(":server  MODE " + it->second.getName() + " +o " + clients[new_cln_index].getNickname());
		}
		++it;
	}

	// Remove pollfd entry
	for (size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i].fd == client_fd)
		{
			fds.erase(fds.begin() + i);
			std::cout << "Pollfd removed, client_fd: "
				<< client_fd
				<< " total fds: "
				<< fds.size()
				<< std::endl;
			break;
		}
	}

	// Remove client object
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getFd() == client_fd)
		{
			clients.erase(clients.begin() + i);
			std::cout << "Client removed, client_fd: "
				<< client_fd
				<< " total clients: "
				<< clients.size()
				<< std::endl;
			break;
		}
	}

	close(client_fd);
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