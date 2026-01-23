/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judith <judith@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:13 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/23 16:57:16 by judith           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

# define MAX_BUFFER_SIZE 2048

class Server;

class Client
{
	public:	
		Client(int client_fd);
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

		int getFd();
		bool getIsInChannel() const;

		void AppendToBuffer(const std::string rec_buffer);
		std::vector<std::string> ExtractCompleteCommands(std::string &input);
		

	private:
		int	client_fd;
		bool isInChannel;
		std::string buffer;
		std::string nickname;
		std::string username;

		

};

#endif