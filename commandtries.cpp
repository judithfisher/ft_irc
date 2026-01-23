// void Server::ProcessCommand(int client_fd, const std::string &line)
// {
// 	std::vector<std::string> tokens = split(line);
// 	if (tokens.empty())
// 		return;
// 	const std::string &command = tokens[0];

// 	std::string pass;
// 	if(tokens.size() > 1)
// 		pass = tokens[1]; // to compare word right after PASS with server password
// 	elase
// 		pass = "";
// 	std::string not_reg_yet = ":server 451 :You have not registered\r\n"
// 	// std::string wrong_cmnd_msg = RED "Wrong command! "R YEL"\nU need to register connection password first. That was ur "<< i+1 << " attempt out of 3\nUse: <PASS> <password>\n" R;
// 	// std::string discon_msg = RED "That was your 3rd wrong attempt to register connection password.\n You are going to be disconnected right now\n" R;
// 	// std::string wrong_pass_msg = RED "Wrong password! "R YEL"\nU need to register correct connection password. That was ur "<< i+1 << " attempt out of 3\nUse: <PASS> <password>\n" R;
// 	// std::string con_msg = GRN "You have been already connected with your connection password\n" R;
// 	// std::string empty_msg = RED "Your password cannot be empty.\nThat was ur "<< i+1 << " attempt out of 3\nUse: <PASS> <password>\n";
// 	// std::string reg_pass_msg = GRN "Connection pasword is correct.\nYou are now connected and registered with connection password\n Pleas set up your nickname and user name.\n" R;

// 	int j;
// 	for (size_t i = 0; i < clients.size(); i++) //find client with fd we r getting connection/input from
// 		if(clients[i].getFd() == client_fd)
// 		{
// 			j = i;
// 			break;
// 		}

// 	if(clients[j].getPassAccepted == false) //check did client registered connection passwrod. Needs to be done as very first thing
// 	{
// 		if(command != "PASS")
// 		{
// 			send(client_fd, not_reg_yet.c_string(), not_reg_yet.size(), 0);

// 			if (i ==2) // disconnect client upon 3rd failure
// 			{
// 				send((client_fd, discon_msg.c_string(), discon_msg.size(), 0));
// 				RemoveClient(client_fd);
// 				return;
// 			}
// 			continue;
// 		}
// 			else if(command == "PASS") // check for password correction
// 			{
// 				if(pass.empty() || ) //empty() returns true if emptyfalse if not empty
// 				{
// 					send(client_fd,empty_msg.c_string(), empty_msg.size(), 0);
// 					if(i == 2)
// 					{
// 						send(client_fd, discon_msg.c_string(), discon_msg.size(), 0);
// 						RemoveClient(client_fd);
// 						return;
// 					}
// 					else
// 						continue;
// 				}
// 				else if(!pass.empty()) //check does password matches the server password
// 				{
// 					if(pass != this->password)
// 					{
// 						send(client_fd, wrong_pass_msg.c_string(), wrong_pass_msg.size(), 0);
// 						if( i == 2)
// 						{
// 							send(client_fd, discon_msg.c_string(), discon_msg.size(), 0);
// 							RemoveClient(client_fd);
// 							return;
// 						}
// 						else
// 							continue;
// 					}
// 					else if(pass == this->password && client[i].getPassAccepted == false)	
// 					{
// 						send(client_fd, reg_pass_msg.c_string(), reg_pass_msg.size(), 0);
// 						clients[j].setPassAcceptedTrue();
// 						clients[j].setPassword(pass);
// 						return;
// 					}
// 					elae if 
				
// 			}
// 		}
// 	}
// 	else if(clients[j].getPassAccepted() == true && command == "PASS") 
// 		send(client_fd, con_msg.c_string(), con_msg.size(), 0);
// 	 //add here if flag of regitration is true
// 	{
// 		if (command == "QUIT")					// just a check
// 		{
// 			std::cout << "Client requested to quit, client_fd: " << client_fd << std::endl;
// 			RemoveClient(client_fd);
// 			return;
// 		}
// 		std::cout << "Received command from client_fd " << client_fd << ": " << command << std::endl; 	// just for testing
		
// 			// if PRIVMSG send message to target
// 			// if JOIN add client to channel
// 			// etc...
// 	}
		







/*  code for NIck nad User same logic separately made at first
		else if(command == "USER")
		{
			if(pass.empty()) //empty() returns true if emptyfalse if not empty
			{
				send(client_fd, need_more_param.c_str(), need_more_param.size(), 0);
				return;
			}
			else if(!pass.empty()) 
			{
				clients[j].setUser(pass);
				if(clients[j].check_nick_user_filled())
				{
					std::string registered = ":server 001 " + clients[j].getNick() + " :Welcome to the IRC network " + clients[j].getNick() + "\r\n";
					clients[j].setRegisteredTrue();
					send(client_fd, registered.c_str(), registered.size(), 0);
				}
				return;
			}	
		}
     */