/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:13 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/31 23:36:49 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>

# define MAX_BUFFER_SIZE 2048

class Client
{
public:
	Client(int client_fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();

	// setters
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setIsInChannel();
	void setPassAccepted();
	void setRegistered();
	void setIsOperator(bool status);

	// getters
	int getFd();
	bool getIsInChannel() const;
	std::string getNickname() const;
	std::string getUsername() const;
	bool getPassAccepted() const;
	bool getIsRegistered() const;
	bool getIsOperator() const;
	int getClientfd(std::string &nick) const;

	void AppendToBuffer(const std::string &rec_buffer);
	std::vector<std::string> ExtractCompleteCommands();

private:
	int client_fd;
	int isOperator;
	bool isInChannel;
	bool passAccepted;
	bool isRegistered;
	std::string buffer;
	std::string nickname;
	std::string username;
};

#endif
