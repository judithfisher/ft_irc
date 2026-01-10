/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:25:24 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 18:17:46 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

Client::Client()
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
		// copy attributes here when added
	}
	return (*this);
}

Client::~Client()
{
}



void Client::acceptClient()
{
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int server_fd = this->server->getServerfd();
	this->client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
	if (this->client_fd < 0)
	{
		std::cerr << "Error accepting client connection" << std::endl;
		return ;
	}
}
