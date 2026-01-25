/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerChannels.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judith <judith@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 18:05:00 by codex             #+#    #+#             */
/*   Updated: 2026/01/24 18:05:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Channel management
// Channels live in a map to keep stable keys and O(log n) lookup by name.
bool Server::channelExists(const std::string &name) const
{
	return (channels.find(name) != channels.end());
}

Channel *Server::getChannel(const std::string &name)
{
	// Map values are owned by Server; return a pointer for in-place mutation.
	std::map<std::string, Channel>::iterator it = channels.find(name);
	if (it != channels.end())
		return &(it->second);
	return NULL;
}

void Server::createChannel(const std::string &name)
{
	// Only create if it doesn't already exist.
	if (!channelExists(name))
	{
		channels.insert(std::make_pair(name, Channel(name)));
		std::cout << "Channel created: " << name << std::endl;
	}
	else
	{
		std::cerr << "Channel already exists: " << name << std::endl;
	}
}

void Server::removeChannel(const std::string &name)
{
	// Explicit removal (also used when a channel becomes empty).
	std::map<std::string, Channel>::iterator it = channels.find(name);
	if (it != channels.end())
	{
		channels.erase(it);
		std::cout << "Channel removed: " << name << std::endl;
	}
	else
	{
		std::cerr << "Channel not found: " << name << std::endl;
	}
}
