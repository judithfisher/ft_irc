/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:27:09 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 18:27:50 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

// 
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ft_irc <port> <password>" << std::endl;
		return (1);
	}
	
	Server server;
	server.setport(std::atoi(argv[1]));
	server.setpassword(argv[2]);

	server.InitServerSocket();
	return (0);
}