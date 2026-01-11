/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:44:51 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/11 19:56:28 by jfischer         ###   ########.fr       */
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

# include "Client.hpp"

class Server
{
	public:	
		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
		
		int getServerfd();
		std::vector<Client> getClientvector();
		
		void setport(int port);
		void setpassword(std::string password);
		
		static void SignalHandler(int signum);

		void InitServerSocket();
		void AcceptClients();
		void ClearClients();

		static bool	SignalReceived;			// part of class server, not individual objects --> static

		class InvalidInput : public std::exception
		{
			const char *what() const throw()
				return "Port argument can only consist digits 0-9.";
		}
		
	private:
		int		port;
		int		server_fd;	
		
		std::string	password;
		std::vector<Client>	clients; 		// to keep track of connected clients + to manage their requests
		std::vector<struct pollfd> fds;		// vector of pollfd, to monitor multiple file descriptors
};

#endif

/*
The <netinet/in.h> header defines the sockaddr_in structure that includes at least the following members:

sa_family_t     sin_family   AF_INET	
in_port_t       sin_port     Port number 
struct in_addr  sin_addr     IP address
*/