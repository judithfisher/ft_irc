/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judith <judith@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:25:24 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/24 16:23:15 by judith           ###   ########.fr       */
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

bool Client::getIsInChannel() const
{
	return (this->isInChannel);
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

void Client::AppendToBuffer(const std::string rec_buffer)
{
	buffer += rec_buffer;
	if (buffer.size() > MAX_BUFFER_SIZE)
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

// "USER test\r\nJOIN #chan\r\nPRIVMSG #chan :Hi\r\n"
// netcat only sends \n not \r\n, so we need to handle that case too
// std::vector<std::string> Client::ExtractCompleteCommands(std::string &input)
// {
// 	std::string token;
// 	std::vector<std::string> commands;
// 	std::istringstream istreami(input);


//     while (istreami >> token)
//         commands.push_back(token);

//     return (commands);


// }

std::vector<std::string> Client::ExtractCompleteCommands()
{
    std::vector<std::string> commands;  // ✅ Vector of LINES (not tokens!)
    
    while (true)
    {
        // Find newline (\r\n or \n)
        size_t pos = buffer.find('\n');
        
        if (pos == std::string::npos)
            break;
        
        // Extract line before \n
        std::string line = buffer.substr(0, pos);
        
        // Remove trailing \r if present
        if (!line.empty() && line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);
        
        // Add non-empty lines
        if (!line.empty())
            commands.push_back(line);  // ✅ Push the WHOLE LINE
        
        // Remove from buffer
        buffer.erase(0, pos + 1);
    }
    
    return commands;  // ✅ Returns ["NICK alice", "USER alice 0 * :Alice", ...]
}
