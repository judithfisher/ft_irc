/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:25:24 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 20:45:49 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

Client::Client(int client_fd): client_fd(client_fd)
{
	std::cout << "This is my client fd: " << this->client_fd << std::endl;
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