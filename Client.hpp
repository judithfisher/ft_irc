/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:13 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 18:10:26 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Server;

class Client
{
	public:	
		Client();
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

	void getServerfd();
		
	void acceptClient();

	private:
		//sockaddr_in client_addr;
		Server *server;
		
		int client_fd;
		std::string nickname;
		std::string username;

		

};

#endif