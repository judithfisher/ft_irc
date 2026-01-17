/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:13 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/17 17:27:07 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Server;

class Client
{
	public:	
		Client(int client_fd);
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

		int getFd();

		void AppendToBuffer(const std::string &rec_buffer);

	private:
		int				client_fd;
		unsigned int   MAX_BUFFER_SIZE;
		
		std::string buffer;
		std::string nickname;
		std::string username;

		

};

#endif