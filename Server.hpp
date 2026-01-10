/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:44:51 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 11:37:05 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Server
{
	public:	
		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
		
		void setport(int port);
		void InitServerSocket();

	private:
		int		port;
		int		server_fd;
		bool	SignalReceived;
		// vector<Client>	clients; 	// to keep track of connected clients + to manage their requests
};