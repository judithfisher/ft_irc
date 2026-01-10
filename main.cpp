/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:27:09 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 11:36:01 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
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
	server.port = std::atoi(argv[1]);
	std::string password = argv[2];

	int server_fd;
	//server_fd = socket();
	return 0;
}