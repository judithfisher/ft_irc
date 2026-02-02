/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 18:05:00 by codex             #+#    #+#             */
/*   Updated: 2026/02/02 20:38:06 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// IRC tokenization with support for the trailing parameter (after ':').
// The trailing parameter can contain spaces (e.g. PRIVMSG #chan :hello world).
std::vector<std::string> Server::ParseCommand(const std::string &line)
{
	std::vector<std::string> tokens;
	std::string current;
	bool in_trailing = false;

	for (size_t i = 0; i < line.length(); i++)
	{
		char c = line[i];

		if (c == ':' && !in_trailing && (i == 0 || line[i - 1] == ' '))
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
			in_trailing = true;
			continue;
		}

		if (c == ' ' && !in_trailing)
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
		}
		else
		{
			current += c;
		}
	}

	if (!current.empty())
		tokens.push_back(current);

	return tokens;
}

void Server::Greeting(int client_fd)
{
	sendLine(client_fd, "375 :- IRC SERVER 42 Message of the Day -");

	sendLine(client_fd, " \033[1;35m░██████░█████████    ░██████       ░██████   ░██████████ ░█████████  ░██    ░██ ░██████████ ░█████████        ░████    ░██████");
	sendLine(client_fd, "   ░██  ░██     ░██  ░██   ░██     ░██   ░██  ░██         ░██     ░██ ░██    ░██ ░██         ░██     ░██      ░██ ██   ░██   ░██");
	sendLine(client_fd, "   ░██  ░██     ░██ ░██           ░██         ░██         ░██     ░██ ░██    ░██ ░██         ░██     ░██     ░██  ██         ░██");
	sendLine(client_fd, "   ░██  ░█████████  ░██            ░████████  ░█████████  ░█████████  ░██    ░██ ░█████████  ░█████████     ░██   ██     ░█████");
	sendLine(client_fd, "   ░██  ░██   ░██   ░██                   ░██ ░██         ░██   ░██    ░██  ░██  ░██         ░██   ░██      ░█████████  ░██");
	sendLine(client_fd, "   ░██  ░██    ░██   ░██   ░██     ░██   ░██  ░██         ░██    ░██    ░██░██   ░██         ░██    ░██          ░██   ░██");
	sendLine(client_fd, " ░██████░██     ░██   ░██████       ░██████   ░██████████ ░██     ░██    ░███    ░██████████ ░██     ░██         ░██   ░████████\033[0m");

	sendLine(client_fd, " ");
	sendLine(client_fd, "\033[1;35mNOTICE * :Welcome to IRC SERVER 42");
	sendLine(client_fd, "\033[1;35mNOTICE * :Authentication required.");
	sendLine(client_fd, "\033[1;35mNOTICE * :");
	sendLine(client_fd, "\033[1;35mNOTICE * :Please authenticate first:");
	sendLine(client_fd, "\033[1;35mNOTICE * :PASS <password>");
	sendLine(client_fd, "\033[1;35mNOTICE * :");
	sendLine(client_fd, "\033[1;35mNOTICE * :Then continue with:");
	sendLine(client_fd, "\033[1;35mNOTICE * :NICK <nickname>");
	sendLine(client_fd, "\033[1;35mNOTICE * :USER <username> 0 * :realname");
}

void Server::HandlePass(int client_fd, const std::vector<std::string> &line, int client_index)
{
	// int client_index = findClientbyFd(client_fd);
	// if (client_index < 0)
	// 	return;
	if (line.size() < 2)
	{
		std::cerr << "Client fd: " << client_fd << " sent PASS without parameter." << std::endl;
		sendLine(client_fd, ":server 461 * PASS :Not enough parameters");
		return;
	}
	std::string pass = line[1];
	if (pass.empty())
	{
		std::cerr << "Client fd: " << client_fd << " provided empty password." << std::endl;
		sendLine(client_fd, RED ":server 464 * :Password incorrect" R);
		return;
	}
	if(clients[client_index].getIsRegistered())/* does not allow to use PASS after registration */
	{
		std::cout << GRN "Client is already registered with nick, user and password" R << std::endl;
		sendLine(client_fd, ":server 462 " + clients[client_index].getNickname() + ":You may not reregister");
		return;
	}
	if((!clients[client_index].getIsRegistered()) && clients[client_index].getPassAccepted())/* if pass set but not registered  PASS should not try to match passwords anymore*/
	{
		/* according to hexchat logic of settign username and nickname right away do we even need this check? */
		std::cout << GRN "Client already registered the password" R << std::endl;
		sendLine(client_fd, ":server 462 " + clients[client_index].getNickname() + ":You may not reregister");
		return;
	}
	if (pass == this->password)
	{
		clients[client_index].setPassAccepted();
		std::cout << "Client fd: " << client_fd << " provided correct password." << std::endl;
	}
	else
	{
		std::cout << "Client fd: " << client_fd << " provided incorrect password." << std::endl;
		sendLine(client_fd, ":server 464 * :Password incorrect");
		return;
	}

}

void Server::HandleNick(int client_fd, const std::vector<std::string> &line, int client_index)
{
	// int client_index = findClientbyFd(client_fd);
	// if (client_index < 0)
	// 	return;
	if (line.size() < 2)
	{
		std::cerr << "Client fd: " << client_fd << " sent NICK without parameter." << std::endl;
		sendLine(client_fd, ":server 431 :No nickname given");
		return;
	}
	std::string nickname = line[1];
	if (clients[client_index].getPassAccepted() == true)
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (clients[i].getNickname() == nickname)
			{
				std::cerr << "Client fd: " << client_fd << " attempted to set duplicate nickname: " << nickname << std::endl;
				sendLine(client_fd, ":server 433 " + nickname + " :Nickname is already in use");
				return;
			}
		}
		if(nickname[0] == '#' || nickname[0] == '&' || nickname[0] == ':' || isdigit(nickname[0]))
		{	
			/* friday added check */
			std::cout << RED "invalid nick: starts from prohibited character." R<< std::endl;
			sendLine(client_fd, ":server 432 " +nickname + " :Erroneous nickname");
			return;
		}
		for(size_t i = 0; i < nickname.size(); i++)
		{
			/* friday added check */
			if((nickname[i] == ' ') || (!isascii(nickname[i])))
			{
				std::cout << RED "invalid nick: space or non ascii char." R << std::endl;
				sendLine(client_fd, ":server 432 " +nickname + " :Erroneous nickname");
				return;
			}
		}
		// if(!(clients[client_index].getNickname().size() > 0))
		if(clients[client_index].getNickname().empty())
		{
			clients[client_index].setNickname(nickname);
			std::cout << "Client fd: " << client_fd << " set nickname to: " << nickname << std::endl;
			if (clients[client_index].getNickname().size() != 0 && clients[client_index].getUsername().size() != 0)
			{
				clients[client_index].setRegistered();
				/* server need to acknowledge registration by seeing somewhere 001 info: */
				sendLine(client_fd, ":server 001 " + clients[client_index].getNickname() + " :Welcome to the ft_IRC server " + clients[client_index]. getNickname());
			}
		}
		
		else
		{
			std::string old_nick = clients[client_index].getNickname();
			clients[client_index].setNickname(nickname);
			for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
			{
				if (it->second.isUserInChannel(client_fd))
				{
					it->second.updateUserNick(client_fd, nickname);
					std::string nickChangeMsg = ":" + old_nick + "!" + clients[client_index].getUsername() + "@host NICK :" + nickname;
					it->second.broadcast(nickChangeMsg);
				}
				// }
			}
		}
	}
	else
		std::cerr << "Client fd: " << client_fd << " attempted to set nickname without passing authentication." << std::endl;
}

void Server::HandleUser(int client_fd, const std::vector<std::string> &line, int client_index)
{
	if (line.size() < 2)
	{
		std::cerr << "Client fd: " << client_fd << " sent USER without parameter." << std::endl;
		return;
	}

	if (clients[client_index].getUsername().size() != 0)
	{
		std::cerr << "Client fd: " << client_fd << " attempted to set username again." << std::endl;
		sendLine(client_fd,":server 462 " + clients[client_index].getNickname() + " :You may not reregister");
		return ;
	}
	
	std::string username = line[1];
	if (clients[client_index].getPassAccepted() == true)
	{
		clients[client_index].setUsername(username);
		std::cout << "Client fd: " << client_fd << " set username to: " << username << std::endl;
		if (clients[client_index].getNickname().size() != 0
			&& clients[client_index].getUsername().size() != 0)
		{
			clients[client_index].setRegistered();
			sendLine(client_fd, ":server 001 " + clients[client_index].getNickname() + " :Welcome to the ft_IRC server " + clients[client_index]. getNickname());
		}
	}
	else
		std::cerr << "Client fd: " << client_fd
			<< " attempted to set username without passing authentication." << std::endl;
}

void Server::HandleJoin(int client_fd, const std::vector<std::string> &line, int client_index)
{
	if (line.size() < 2)
	{
		// 461: ERR_NEEDMOREPARAMS
		sendLine(client_fd, ":server 461 JOIN :Not enough parameters");
		return;
	}

	std::string channel_name = line[1];

	if (channel_name.empty() || channel_name[0] != '#')
	{
		// 403: ERR_NOSUCHCHANNEL
		sendLine(client_fd, "403 " + channel_name + " :No such channel");
		return;
	}

	if (!channelExists(channel_name))
		createChannel(channel_name);

	Channel *ch = getChannel(channel_name);
	if (!ch)
		return;

	if (ch->isUserInChannel(client_fd))
	{
		std::cout << "Already in channel" << std::endl;
		return;
	}

	// +i (invite-only): allow only first user, all others must be invited
	// Only the first user can join a +i channel without invite
	if (ch->isInviteOnly() && ch->getUserCount() > 0)
	{
		// if (ch->getUserCount() == 0) /* MONDAY for emopty chat situation */
		// 	ch->addUser(client_fd, clients[client_index].getNickname());

		if (!ch->isUserInvited(client_fd)) 
		{
			sendLine(client_fd, ":server 473 " + channel_name + " :Cannot join channel (+i)");
			return;
		}
		ch->removeInvitedUser(client_fd);
	}
	
	if (ch->hasPassword()) 
	{
		if (line.size() < 3 || line[2] != ch->getPassword()) 
		{
			sendLine(client_fd, ":server 475 " + channel_name + " :Cannot join channel (+k) - bad key");
			return;
		}
	}
	 /* +l (limit)
	 forcing limit for users */
	if (ch->getUserLimit() > 0 && ch->getUserCount() >= (size_t)ch->getUserLimit()) 
	{
		sendLine(client_fd, ":server 471 " + channel_name + " :Cannot join channel (+l)");
		return;
	}

	// Store fd + nick in channel to avoid Client* pointer invalidation.
	ch->addUser(clients[client_index].getFd(), clients[client_index].getNickname());

	if (ch->getUserCount() == 1)
		ch->addOperator(client_fd);

	clients[client_index].setIsInChannel();

	std::string nick = clients[client_index].getNickname();
	std::string user = clients[client_index].getUsername();
	if (user.empty())
		user = "user";

	// 1) Broadcast JOIN to all members
	std::string joinMsg = ":" + nick + "!" + user + "@host JOIN " + channel_name;
	const std::map<std::string, int> &members = ch->getClients();
	for (std::map<std::string, int>::const_iterator it = members.begin();
		it != members.end(); ++it)
	{
		sendLine(it->second, joinMsg);
	}

	// 2) Topic (only to joining user)
	// 331: RPL_NOTOPIC
	sendLine(client_fd, ":server 331 " + nick + " " + channel_name + " :No topic is set");

	// 3) Names list (only to joining user)
	// 353: RPL_NAMREPLY
	std::string names = ":server 353 " + nick + " = " + channel_name + " :";
	for (std::map<std::string, int>::const_iterator it = members.begin();
		it != members.end(); ++it)
	{
		if (ch->isOperator(it->second))
			names += "@";
		names += it->first;
		names += " ";
	}
	std::cout << "Names: " << names << std::endl;
	sendLine(client_fd, names);

	// 4) End of names
	// 366: RPL_ENDOFNAMES
	sendLine(client_fd, ":server 366 " + nick + " " + channel_name + " :End of /NAMES list");

	std::cout << "Client " << nick << " joined " << channel_name << std::endl;
}

void Server::HandlePrivMsg(int client_fd, const std::vector<std::string> &line, int client_index)
{
	if (line.size() < 2)
	{
		// 411: ERR_NORECIPIENT
		sendLine(client_fd, ":server 411 : No recipient given (PRIVMSG)");
		return;
	}
	if (line.size() < 3)
	{
		// 412: ERR_NOTEXTTOSEND
		sendLine(client_fd, ":server 412 : No text to send");
		return;
	}
	
	std::string target = line[1];
	std::string message = line[2];
	for (size_t i = 3; i < line.size(); i++)
	{
			if (i > 2)
				message += " " + line[i];
			//message = message + line[i];
	}

	std::string nick = clients[client_index].getNickname();
	std::string user = clients[client_index].getUsername();
	if (user.empty())
		user = "user";

	std::string payload = ":" + nick + "!" + user + "@host PRIVMSG " + target + " :" + message;

	if (!target.empty() && target[0] == '#')		// if it's a channel
	{
		Channel *ch = getChannel(target);
		if (!ch)
		{
			// 403: ERR_NOSUCHCHANNEL
			sendLine(client_fd, ":server 403 " + target + " :No such channel");
			return;
		}
		if (!ch->isUserInChannel(client_fd))
		{
			// 404: ERR_CANNOTSENDTOCHAN
			sendLine(client_fd, ":server 404 " + target + " :Cannot send to channel");
			return;
		}

		const std::map<std::string, int> &members = ch->getClients();
		for (std::map<std::string, int>::const_iterator it = members.begin(); it != members.end(); it++)			// send to all members in ch(annel)
		{
			int target_fd = it->second;
			if (target_fd != client_fd)																				// except sender					
				sendLine(target_fd, payload);
		}
		std::cout << "Client fd: " << client_fd << " sent PRIVMSG to " << target << ": " << message << std::endl;
		return;
	}

	for (size_t i = 0; i < clients.size(); i++)																		// outside if target[0] == '#'], so it's a user	
	{
		if (clients[i].getNickname() == target)
		{
			sendLine(clients[i].getFd(), payload);
			std::cout << "Client fd: " << client_fd << " sent PRIVMSG to " << target << ": " << message << std::endl;
			return ;
		}
	}

	// 401: ERR_NOSUCHNICK
	sendLine(client_fd, ":server 401 " + target + " :No such nick/channel");												// otherwise, no channel nor user found
}

void Server::HandleTopic(int client_fd, const std::vector<std::string> &line, int client_index)
{		
	if (line.size() < 2)
	{
		sendLine(client_fd, ":server 461 TOPIC :Not enough parameters");				
		return ;
	}
	
	if (clients[client_index].getIsInChannel() == false)
	{
		sendLine(client_fd, ":server 442 :You're not on that channel");
		return ;
	}
	
	Channel *ch = getChannel(line[1]);
	if (!ch)
		return ;
		
	/* MODE */
	std::cout << "HEEERRRREEEEE" << std::endl;
	if (ch->getTopicRestricted() == true && !ch->isOperator(client_fd))
	{
		std::cout << "NOOOOOWWWWWW" << std::endl;
		sendLine(client_fd, ":server 482 " + line[1] + " :You're not channel operator");
		return ;
	}
	std::string nick = clients[client_index].getNickname();
	if (line.size() == 2)		
	{
		std::string current_topic = ch->getTopic();
		if (!current_topic.empty())
		{
			// ch->setTopicRestricted(true); //we just seeing here as i understand
			sendLine(client_fd, ":server 332 " + nick + " " + line[1] + " :" + current_topic);							// 332: RPL_TOPIC
			return ;
		}
		sendLine(client_fd, ":server 331 " + nick + " " + line[1] + " :No topic is set");
		return ;
	}


	std::string topic = line[2];
	ch->setTopic(topic);
	std::map<std::string, int> members = ch->getClients();
	for (std::map<std::string, int>::iterator it = members.begin(); it != members.end(); ++it)
		sendLine(it->second, ":" + nick + "!"+ clients[client_index].getUsername() + "@host TOPIC " + line[1] + " :" + topic);
	
	std::cout << "Client fd: " << client_fd << " set TOPIC for channel " << line[1] << " to: " << topic << std::endl;
}

void Server::HandleInvite(int client_fd, const std::vector<std::string> &line, int client_index)
{
	for(int i = 0; i < (int)line.size(); i++)
	if (line.size() < 3)
	{
		sendLine(client_fd, ":server 461 INVITE :Not enough parameters");
		return ;
	}
	
	if (clients[client_index].getIsInChannel() == false)
	{
		sendLine(client_fd, ":server 442 :You're not on that channel");
		return ;
	}
	
	if (line[2][0] != '#')
	{
		sendLine(client_fd, ":server 403 " + line[2] + " :No such channel");
		return ;
	}
	
	std::string channel_name = line[2];
	Channel *ch = getChannel(channel_name);
	if (!ch)
		return ;

	std::string target_nick = line[1];
	int target_fd = -1;
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getNickname() == target_nick)
		{
			target_fd = clients[i].getFd();
			break ;
		}
	}
	
	if (target_fd == -1)
	{
		sendLine(client_fd, ":server 401 " + target_nick + " :No such nick/channel");
		return ;
	}
	
	if (ch->isInviteOnly() && !ch->isOperator(client_fd))
	{
    	sendLine(client_fd, ":server 482 " + channel_name + " :You're not channel operator");
    	return;
	}

	ch->addInvitedUser(target_fd);

	sendLine(target_fd, ":" + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@host INVITE " + target_nick + " " + channel_name);
	std::cout << "Client fd: " << client_fd << " invited " << target_nick << " to channel " << channel_name << std::endl;
}

void Server::HandleKick(int client_fd, const std::vector<std::string> &line, int client_index)
{
	if (line.size() < 3)
	{
		sendLine(client_fd, ":server 461 KICK :Not enough parameters");
		return ;
	}
	// int client_index = findClientbyFd(client_fd);
	// if (client_index < 0)	
	// 	return ;

	// if (clients[client_index].getIsRegistered() == false)
	// {
	// 	sendLine(client_fd, "451 :You have not registered");
	// 	return ;
	// }
	
	if (clients[client_index].getIsInChannel() == false)
	{
		sendLine(client_fd, ":server 442 :You're not on that channel");
		return ;
	}
	
	std::string channel_name = line[1];
	Channel *ch = getChannel(channel_name);
	if (!ch)
		return ;
	
	if (!ch->isOperator(client_fd))
	{
		sendLine(client_fd, ":server 482 :You're not channel operator");
		return ;
	}
	std::string target_nick = line[2];
	int target_fd = -1;
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getNickname() == target_nick)
		{
			target_fd = clients[i].getFd();
			break ;
		}
	}
	if (target_fd == -1 || !ch->isUserInChannel(target_fd))
	{
		sendLine(client_fd, "441 " + target_nick + " " + channel_name + " :They aren't on that channel");
		return ;
	}
	
	std::string kick_msg = ":" + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@host KICK " + channel_name + " " + target_nick;
	const std::map<std::string, int> &members = ch->getClients();
	for (std::map<std::string, int>::const_iterator it = members.begin(); it != members.end(); ++it)
		sendLine(it->second, kick_msg);
	
	ch->removeUser(target_fd);
	std::cout << "Client fd: " << client_fd << " kicked " << target_nick << " from channel " << channel_name << std::endl;
}

/* IRS does not read ANSI code it will either ignore or send gibberish so no colors in sendLine (send) */
void Server::HandleQuit(int client_fd, int client_index, std::vector<std::string> tokens)
{
	/* broadcast message to all channels */
	std::string nick = clients[client_index].getNickname();
	std::string user = clients[client_index].getUsername();
	std::string msg;
	if(tokens.size() == 1)
		msg = ":Client Quit";
	else if (tokens.size() > 1)
	{
		if(tokens[1][0] != ':')
			msg = ":" + tokens[1];
		else
			msg = tokens[1];
		for(size_t i = 2; i < tokens.size(); i++)
			msg += " " + tokens[i];
	}
	/* in this moment we added if statement. was working(?) without if previously just inside of if walid */
	if (clients[client_index].getIsInChannel())
	{
		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
			if(it->second.isUserInChannel(client_fd))
				it->second.broadcast(":" + nick + "!" + user + "@host QUIT " + msg);
	}

	std::cout << "Client fd: " << client_fd << " is quitting." << std::endl;
	sendLine(client_fd, "\033[2J\033[H");  // Clear screen
	RemoveClient(client_fd);
}

//MODE
void Server::HandleMode(int client_fd, const std::vector<std::string> &line, int client_index)
{
	if (line.size() < 2) 
	{
		sendLine(client_fd, ":server 461 MODE :Not enough parameters");
		return;
	}
	std::string channelName = line[1];
	Channel *channel = getChannel(channelName);
	if (!channel) 
	{
		sendLine(client_fd, ":server 403 " + channelName + " :No such channel");
		return;
	}
	// int clientIndex = findClientbyFd(client_fd);
	// if (clientIndex == -1) 
	// 	return;
	if (!channel->isOperator(client_fd)) 
	{
		sendLine(client_fd, ":server 482 " + channelName + " :You're not channel operator");
		return;
	}
	if (line.size() == 2) 
	{
		std::string modes = "+";
		if (channel->isInviteOnly()) 
			modes += "i";
		if (channel->getTopicRestricted()) 
			modes += "t";
		if (channel->hasPassword()) 
			modes += "k";
		if (channel->getUserLimit() > 0) 
			modes += "l";
		sendLine(client_fd, ":server 324 " + channelName + " " + modes);
		return;
	}
	std::string modeStr = line[2];
	bool adding = true;
	size_t paramIndex = 3;
	bool modeChanged = false;		/* to print message only when mode is correctly used, not every time  it tries */
	for (size_t i = 0; i < modeStr.size(); ++i) 
	{
		char mode = modeStr[i];
		if (mode == '+') 
			adding = true;
		else if (mode == '-')
			adding = false;
		else if (mode == 'i')
		{
			channel->setInviteOnly(adding);
			modeChanged = true;
		}
		else if (mode == 't')
		{
			channel->setTopicRestricted(adding);
			modeChanged = true;
		}
		else if (mode == 'k') 
		{
			if (adding) 
			{
				if (line.size() > paramIndex)
				{
					channel->setPassword(line[paramIndex++]);
					modeChanged = true;
				}
				else
					sendLine(client_fd, ":server 461 MODE :Not enough parameters for +k");
			}
			else
			{
				channel->removePassword();
				modeChanged = true;
			}
		} 
		else if (mode == 'o') 
		{
			if (line.size() > paramIndex) 
			{
				int targetFd = -1;
				std::string nick = line[paramIndex++];
				for (size_t u = 0; u < clients.size(); ++u) 
				{
					if (clients[u].getNickname() == nick)
						targetFd = clients[u].getFd();
				}
				if (targetFd == -1) 
				{
					sendLine(client_fd, ":server 401 " + nick + " :No such nick");
				}
				else 
				{
					if (adding)/* MONDAY  */
					{
						channel->addOperator(targetFd);
						std::string modeMsg = ":" + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@host MODE " + channel->getName() + " +o " + clients[findClientbyFd(targetFd)].getNickname();
						channel->broadcast(modeMsg);
					}
					else
					{
						channel->removeOperator(targetFd);
						std::string modeMsg = ":" + clients[client_index].getNickname() + "!" + clients[client_index].getUsername() + "@host MODE " + channel->getName() + " -o " + clients[findClientbyFd(targetFd)].getNickname();
						channel->broadcast(modeMsg);
					}
					modeChanged = true;
				}
			}
			else
				sendLine(client_fd, ":server 461 MODE :Not enough parameters for o");
		}
		else if (mode == 'l') 
		{
			if (adding) 
			{
				if (line.size() > paramIndex) 
				{
					int limit = atoi(line[paramIndex++].c_str());
					if(limit < 1)
					{
						sendLine(client_fd, ":server 461 " + clients[client_index].getNickname() + " MODE :Invalid limit for +l");
						continue;
					}
					channel->setUserLimit(limit);
					modeChanged = true;
				} 
				else
					sendLine(client_fd, ":server 461 MODE :Not enough parameters for +l");
			}
			else
			{
				channel->removeUserLimit();
				modeChanged = true;
			}
		}
		else
		{
			std::string m = std::string (1, mode);
			sendLine(client_fd, ":server 472 " + clients[client_index].getNickname() + " " + m + " :is unknown mode char to me");
		}
    }
	if(modeChanged)
	{
		std::string nick = clients[client_index].getNickname();
		std::string user = clients[client_index].getUsername();
		if(user.empty())
			user = "user";
		std::string modeMsg = ":" + nick + "!" + user + "@host MODE " + channelName + " " + modeStr;
		channel->broadcast(modeMsg);
	}
}

void Server::SendMessage(int client_fd, const std::vector<std::string> &line, int client_index)
{
	// int client_index = findClientbyFd(client_fd);
	// if (client_index < 0)
	// 	return;
	if (clients[client_index].getIsInChannel() == true)
	{
		for (size_t i = 1; i < line.size(); i++)
			std::cout << line[i] << std::endl;
	}
	else
	{
		std::cerr << "Client fd: " << client_fd
			<< " attempted to send message without being in a channel." << std::endl;
	}
}

void Server::ProcessCommand(int client_fd, const std::string &line)
{
	if (line.empty())
		return;

	std::vector<std::string> tokens = ParseCommand(line);
	if (tokens.empty())
		return;

	int client_index = findClientbyFd(client_fd);
	if (client_index < 0)
		return;

	std::string command = tokens[0];
	for (size_t i = 0; i < command.size(); i++)
		command[i] = std::toupper(command[i]);
	// IRC commands are case-insensitive.

	std::cout << "[" << client_fd << "] " << command << std::endl;

	if (command == "CAP")
	{
		return; //ignoring 
		/*sendLine(client_fd, "CAP * LS :"); //hexchat. 
		^
		 chat sais this is wrong bc this is client request 
		not a server reply. It suggest to ignore or just sent "CAP * ACK :" */
		
		// sendLine(client_fd, ":server CAP * LS :multi-prefix"); //irssi
	}
	else if (command == "PING")
	{
		if (tokens.size() > 1)
			sendLine(client_fd, "PONG ircserv :" + tokens[1]); /* !!!!!!! tokens.back() !!!!!!also sends last token, safer than token[1] */
		else
			sendLine(client_fd, "PONG ircserv");
	}
	else if (command == "PASS")
		HandlePass(client_fd, tokens, client_index);
	else if (command == "NICK")
		HandleNick(client_fd, tokens, client_index);
	else if (command == "USER")
		HandleUser(client_fd, tokens, client_index);
	else if (command == "QUIT")
		HandleQuit(client_fd, client_index, tokens);
	/* one check for all Commands */
	else if(clients[client_index].getIsRegistered() == true)
	{
		std::cout << "ENTERING COMMANDS BLOCK" << std::endl;
		if (command == "MODE")
			HandleMode(client_fd, tokens, client_index);
		else if (command == "JOIN")
			HandleJoin(client_fd, tokens, client_index);
		else if (command == "PRIVMSG")
			HandlePrivMsg(client_fd, tokens, client_index);
		else if (command == "TOPIC")
			HandleTopic(client_fd, tokens, client_index);
		else if (command == "INVITE")
			HandleInvite(client_fd, tokens, client_index);
		else if (command == "KICK")
			HandleKick(client_fd, tokens, client_index);
		else if (clients[client_index].getIsInChannel())
			SendMessage(client_fd, tokens, client_index);
		else
		{/* this should not be silent (requiers numeric info 421) 
			so as i understand hexchat will send commands we dont support*/
			std::cout << "Unknown command: " << command << std::endl;
			sendLine(client_fd, "421 "+clients[client_index].getNickname() + " " + command + " :Unknown command");
			return;
		}
	}
	else if(!(clients[client_index].getIsRegistered()) && command != "USER" && command != "NICK" && command != "PASS")/* if nto registered cannot use any command */
	{
		std::cout << RED "Prohibited usage!" R YEL "Client nickname: " << clients[client_index].getNickname() << ". Tried to use command: " << command << ", but is not registered yet." R << std::endl;
		sendLine(client_fd, "451 :You have not registered");
		return;
	}
}



