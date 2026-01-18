/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:25:24 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/18 14:49:48 by jfischer         ###   ########.fr       */
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
// netcat only sends \n not \r\n, so we need to handle that case too
std::vector<std::string> Client::ExtractCompleteCommands()
{
	std::vector<std::string> commands;

	while (true)
	{
		size_t pos = buffer.find('\n');
		
		if (pos == std::string::npos)
			break;
	
		std::string line = buffer.substr(0, pos); 				// Extract command up to \n
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);						// Remove trailing \r if present

		if (!line.empty())
			commands.push_back(line); 							// Store the complete command
		buffer.erase(0, pos + 1); 								// Remove extracted command from buffer including \n
	}
	return (commands);
}