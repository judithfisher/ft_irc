/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 11:40:06 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
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

void Server::setport(int port)
{
	this->port = port;
}

void Server::InitServerSocket()
{
	
}