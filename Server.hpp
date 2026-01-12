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

#define R "\033[0m"
#define RED "\033[38;2;239;68;68m"
#define GRN "\033[38;2;34;197;94m"
#define BLU "\033[38;2;59;130;246m"
#define YEL "\033[38;2;234;179;8m"
#define PRP "\033[38;2;168;85;247m"
#define ORN "\033[38;2;255;165;0m"
#define BOLD "\033[1m"

class Server
{
	private:
		int		port;
		int		server_fd;	
		
		std::string	password;
		std::vector<Client>	clients; 		// to keep track of connected clients + to manage their requests
		std::vector<struct pollfd> fds;		// vector of pollfd, to monitor multiple file descriptors
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

		//For try catch blockm
		class InvalidInput : public std::exception
		{
			public:
				const char *what() const throw();
		};

		class InvalidArgsAmount : public std::exception
		{
			public:
				const char*what() const throw();
		};
		
};

#endif

/*
The <netinet/in.h> header defines the sockaddr_in structure that includes at least the following members:

sa_family_t     sin_family   AF_INET	
in_port_t       sin_port     Port number 
struct in_addr  sin_addr     IP address
*/