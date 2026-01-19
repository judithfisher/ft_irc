/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:13 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/17 19:34:24 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

# define MAX_BUFFER_SIZE 2048

class Server;

class Client
{
	private:
		int			client_fd;
		std::string buffer;
		std::string password;
		std::string nickname;
		std::string username;
		bool passAccepted;
		bool registered;

	public:	
		Client(int client_fd);
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

		int getFd();
		bool getPassAccepted() const;
		void setPassAcceptedTrue();
		bool getRegistered() const;
		void setRegisteredTrue();
		void setPassword(const std::string &pass);
		void setNick(const std::string nick);
		void setUser(const std::string user);
		bool check_nick_user_filled();
		const std::string& getNick() const;
		const std::string& getUser() const;
		// const std::string& getNick(); //return info by constant reference to not modyfie anything by accident 
		// const std::string& getUser();


		void AppendToBuffer(const std::string &rec_buffer);
		std::vector<std::string> ExtractCompleteCommands();
};

#endif