/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:44:51 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 13:19:06 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib> 				//for atoi()
# include <vector> 					//for vector
# include <sys/socket.h> 			//for socket()
# include <sys/types.h> 			//for socket()
# include <netinet/in.h> 			//for sockaddr_in
# include <fcntl.h> 				//for fcntl()
# include <unistd.h> 				//for close()
# include <arpa/inet.h> 			//for inet_ntoa()
# include <poll.h> 					//for poll()
# include <csignal> 				//for signal()

class Server
{
	public:	
		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
		
		void setport(int port);
		void setpassword(std::string password);
		
		void InitServerSocket();
		
		//void ClearClients();

	private:
		int		port;
		int		server_fd;
		//bool	SignalReceived;
		
		std::string	password;
		// vector<Client>	clients; 	// to keep track of connected clients + to manage their requests
};

#endif

/*
The <netinet/in.h> header defines the sockaddr_in structure that includes at least the following members:

sa_family_t     sin_family   AF_INET	
in_port_t       sin_port     Port number 
struct in_addr  sin_addr     IP address
*/