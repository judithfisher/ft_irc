/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:27:09 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/09 17:27:04 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

// 
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ft_irc <port> <password>" << std::endl;
		return (1);
	}
	int port = std::atoi(argv[1]);
	std::string password = argv[2];

	int server_fd;
	//server_fd = socket();
	return 0;
}