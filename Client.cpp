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

Client::Client(int client_fd): client_fd(client_fd), passAccepted(false), registered(false)
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
		this->password = other.password;
		this->passAccepted = other.passAccepted;
		this->registered = other.registered;
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

bool Client::getPassAccepted() const
{
	return (this->passAccepted);
}

void Client::setPassAcceptedTrue()
{
	this->passAccepted = true;
}

bool Client::getRegistered() const
{
	return this->registered;
}

void Client::setRegisteredTrue()
{
	this->registered = true;
}

void Client::setPassword(const std::string &pass)
{
	this->password = pass;
}

void Client::setNick(const std::string nick)
{
	this->nickname = nick;
}

void Client::setUser(const std::string user)
{
	this->username = user;
}

void setRealname(const std::string real)
{
	this->realname = real;
}

bool Client::check_nick_user_filled()
{
	if ((!this->nickname.empty()) && (!this->username.empty()))
	return true;
else
return false;
}

const std::string& Client::getNick() const
{
	return (this->nickname);
}
const std::string& Client::getUser() const
{
	return(this->username);
}

const std::string& getRealname() const
{
	return(this->realname);	
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