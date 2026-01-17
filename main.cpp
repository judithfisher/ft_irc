/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:27:09 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/17 13:49:58 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "checkers.hpp"
#include <string>
#include <iostream>
#include <sstream>

bool Server::SignalReceived = false;

int main(int argc, char **argv)
{
	Server server;

	signal(SIGINT, Server::SignalHandler);
	signal(SIGTERM, Server::SignalHandler);

	try //if gonna use try catch here needs to be main init
	{

		check(argc, argv);
		server.setport(std::atoi(argv[1]));
		server.setpassword(argv[2]);
		server.InitServerSocket();
		server.RunServer();
		// while (!Server::SignalReceived)
		// {
		// 	server.AcceptClients();
		// 	// if (server.getClientvector().size() == 100)
		// 	// {
		// 	// 	break;
		// 	// }		
		// }
	}
	
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		server.ClearClients();
		close(server.getServerfd());
		std::cout << GRN "server has been closed" R << std::endl;
		return (1);
	}

	server.ClearClients();
	close(server.getServerfd());
	std::cout << GRN "server has been closed due to signal reception" R << std::endl;

	return (0);
}