/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:25:24 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/17 21:23:30 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

Client::Client(int client_fd): client_fd(client_fd)
{
	// std::cout << "This is my client fd: " << this->client_fd << std::endl;
}	

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->client_fd = other.client_fd;
		this->nickname = other.nickname;
		this->username = other.username;
		// copy attributes here when added
	}
	return (*this);
}

Client::~Client()
{
}

int Client::getFd()
{
	return (this->client_fd);
}

void Client::AppendToBuffer(const std::string &rec_buffer)
{
	buffer += rec_buffer;
	if (buffer.size() > MAX_BUFFER_SIZE)
		throw std::runtime_error("No spamming allowed, you fool! Exceeded maximum buffer size.");
}

// "USER test\r\nJOIN #chan\r\nPRIVMSG #chan :Hi\r\n"
std::vector<std::string> Client::ExtractCompleteCommands()
{
	std::vector<std::string> commands;
	size_t pos;
	std::string line;
	
	
	while ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		line = buffer.substr(0, pos); 				// Extract command up to \r\n
		commands.push_back(line); 					// Store the complete command
		buffer.erase(0, pos +2); 					// Remove extracted command from buffer including \r\n
	}
	return (commands);
}