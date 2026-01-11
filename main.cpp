/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:27:09 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/11 13:45:18 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <string>
#include <iostream>
#include <sstream>

bool Server::SignalReceived = false;

int main(int argc, char **argv)
{
	std::string input;
	if (argc != 3)
	{
		std::cerr << "Usage: ./ft_irc <port> <password>" << std::endl;
		return (1);
	}
	
	Server server;
	server.setport(std::atoi(argv[1]));
	server.setpassword(argv[2]);

	// try
	// {
	// 	signal(SIGINT, Server::SignalHandler);
	// 	signal(SIGQUIT, Server::SignalHandler);
	// 	server.InitServerSocket();
	// 	server.AcceptClients();
	// 	/* code */
	// }
	// catch(const std::exception& e)
	// {
	// 	server.ClearClients();
	// 	close(server.getServerfd());
	// 	std::cerr << e.what() << std::endl;
	// }
	
	server.InitServerSocket();
	
	while (!Server::SignalReceived)
	{
		server.AcceptClients();
		// if (server.getClientvector().size() == 100)
		// {
		// 	break;
		// }		
	}

	// if (Server::SignalReceived)
	// {
		std::cout << "Server shut down gracefully." << std::endl;
		server.ClearClients();
		close(server.getServerfd());
		
	// }
	return (0);
}