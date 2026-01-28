void Server::ProcessCommand(int client_fd, const std::string &line)
{
	std::vector<std::string> tokens = split(line);
	if (tokens.empty())
		return;
	const std::string &command = tokens[0];

	std::string pass;
	if(tokens.size() > 1)
		pass = tokens[1]; // to compare word right after PASS with server password
	else
		pass = "";
	
	std::string not_reg_yet = ":server 451 :You have not registered\r\n";
	std::string unknown_command = ":server 421 " + command + " :Unknown command\r\n";
	std::string need_more_param = ":server 461 " + command + " :Not enough parameters\r\n";
	std::string wrong_pass = ":server 464 * :Password incorrect\r\n";

	int j = 0;
	for (size_t i = 0; i < clients.size(); i++) //find client with fd we r getting connection/input from
		if(clients[i].getFd() == client_fd)
		{
			j = i;
			break;
		}

	if(clients[j].getPassAccepted() == false) //check did client registered connection passwrod. Needs to be done as very first thing
	{
		if(command != "PASS")
		{
			send(client_fd, not_reg_yet.c_str(), not_reg_yet.size(), 0);
			return;
		}
		else if(command == "PASS") // check for password correction
		{
			if(pass.empty()) //empty() returns true if emptyfalse if not empty
			{
				send(client_fd, need_more_param.c_str(), need_more_param.size(), 0);
				return;
			}
			else if(!pass.empty()) //check does password matches the server password
			{
				if(pass != this->password)
				{
					send(client_fd, wrong_pass.c_str(), wrong_pass.size(), 0);
					return;
				}
				else if(pass == this->password)	
				{
					clients[j].setPassword(pass);
					clients[j].setPassAcceptedTrue();
					return;
				}
			}		
		}
	}
	else if(clients[j].getPassAccepted() == true && clients[j].getRegistered() == false) 
	{
		if(command != "NICK" && command != "USER") //if no one of those command then info and again
		{
			send(client_fd, not_reg_yet.c_str(), not_reg_yet.size(), 0);
			return;
		}
		else if(command == "NICK" || command == "USER")
		{
			
			if(pass.empty()) //empty() returns true if empt,y false if not empty
			{
				send(client_fd, need_more_param.c_str(), need_more_param.size(), 0);
				return;
			}
			else if(!pass.empty()) //check does password matches the server password
			{
				if(command == "NICK")
				{
					//checking all registered clients nicks, if double return error msg
					for(int i = 0; i < j; i++)
						if(clients[i].getNick() == pass)
						{
							std::string nick_taken = ":server 433 * " + pass + " :Nickname is already in use\r\n";
							send(client_fd, nick_taken.c_str(), nick_taken.size(), 0);
							return;
						}
					clients[j].setNick(pass);
					std::cout << clients[j].getNick() << std::endl; //TESTING only
				}
				else if (command == "USER")
				{
					// if(tokens.size() < 5)
					// 	send(client_f)
					clients[j].setUser(pass);
					std::cout << clients[j].getUser() << std::endl; //TESTING only
				}
				if(clients[j].check_nick_user_filled())
				{
					std::string registered = ":server 001 " + clients[j].getNick() + " :Welcome to the IRC network " + clients[j].getNick() + "\r\n";
					clients[j].setRegisteredTrue();
					send(client_fd, registered.c_str(), registered.size(), 0);
				}
				return;
			}	
		}		
	}


}
