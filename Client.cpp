/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:25:24 by jfischer          #+#    #+#             */
/*   Updated: 2026/02/01 14:22:23 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sstream>
#include <stdexcept>

Client::Client(int client_fd): client_fd(client_fd), isOperator(false), isInChannel(false), passAccepted(false), isRegistered(false)
{
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
		this->isInChannel = other.isInChannel;
		this->passAccepted = other.passAccepted;
		this->isRegistered = other.isRegistered;
		this->buffer = other.buffer;
		this->nickname = other.nickname;
		this->username = other.username;
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

bool Client::getIsInChannel() const
{
	return (this->isInChannel);
}

bool Client::getIsOperator() const
{
	return (this->isOperator);
}

void Client::setNickname(const std::string &nickname)
{
	this->nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	this->username = username;
}

void Client::setIsInChannel()
{
	this->isInChannel = true;
}

void Client::setPassAccepted()
{
	this->passAccepted = true;
}

void Client::setRegistered()
{
	this->isRegistered = true;
}

void Client::setIsOperator(bool status)
{
	this->isOperator = status;
}

std::string Client::getUsername() const
{
	return (this->username);
}

std::string Client::getNickname() const
{
	return (this->nickname);
}


bool Client::getPassAccepted() const
{
	return (this->passAccepted);
}

bool Client::getIsRegistered() const
{
	return (this->isRegistered);
}

void Client::AppendToBuffer(const std::string &rec_buffer)
{
	this->buffer += rec_buffer;
	if (this->buffer.size() > MAX_BUFFER_SIZE)
		throw std::runtime_error("No spamming allowed, you fool! Exceeded maximum buffer size.");
}

std::vector<std::string> split(const std::string &input)
{
    std::vector<std::string> tokens;
    std::istringstream istreami(input);
    std::string t;

    while (istreami >> t)
        tokens.push_back(t);

    return (tokens);
}

std::vector<std::string> Client::ExtractCompleteCommands()
{
	std::vector<std::string> commands;

	while (true)
	{
		// Find newline (\r\n or \n)
		size_t pos = this->buffer.find('\n');

		if (pos == std::string::npos)
			break;

		// Extract line before \n
		std::string line = this->buffer.substr(0, pos);

		// Remove trailing \r if present
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);

		// Add non-empty lines
		if (!line.empty())
			commands.push_back(line);

		// Remove from buffer
		this->buffer.erase(0, pos + 1);
	}

	return (commands);
}
