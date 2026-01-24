/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judith <judith@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/24 16:53:43 by judith           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


Server::Server()
{
}

Server::Server(const Server &other)
{
	*this = other;
}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->port = other.port;
		this->server_fd = other.server_fd;
		// this->clients = other.clients;
	}
	return (*this);
}

Server::~Server()
{
}

void Server::SignalHandler(int signum)
{
	(void) signum;
	SignalReceived = true;	
}

int Server::getServerfd()
{
	return (this->server_fd);
}

std::vector<Client> Server::getClientvector()
{
	return (clients);
}

void Server::setport(int port)
{
	this->port = port;
}

void Server::setpassword(std::string password)
{
	this->password = password;
}

// AF_INET (Address Family Internet) = IPv4
// Sock_Stream = reliable, ordered, connection-based byte stream = TCP
// htons = host to network short, 16 bits (port 6667-> 0x1A0B) 00001A0B -> 0B1A

// fcntl = file control, manipulate file descriptor
// O_NONBLOCK = non-blocking mode, I/O operations on the socket will be non-blocking
// accept(), rec(), send return immediately with data or error instead of blocking


void Server::InitServerSocket()
{
	sockaddr_in addr;							// from library, structure to hold server address information	
	struct pollfd NewPoll;						// from library, structure for poll monitoring, tells poll which fds to monitor and what events to look for
	
	addr.sin_family = AF_INET;					// IPv4
	addr.sin_port = htons(this->port);			// port number in network byte order
	addr.sin_addr.s_addr = INADDR_ANY;			// accept connections from any IP address
	
	// this->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // IPv4, TCP, explicit protocol
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol (kernel chooses TCP for SOCK_STREAM)
	if (this->server_fd == -1)
	{
		std::cerr << "Socket creation failed" << std::endl;
		return;
	}

	int en = 1;
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) < 0) //Allows reusing the same port immediately after program restart
	{
		std::cerr << "Setsockopt failed" << std::endl;
		return;
	}

	if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)	// all I/O operations on the socket will be non-blocking ( read accept do not freez server when no data is available)
	{
		std::cerr << "Failed to set non-blocking mode" << std::endl;
		close (this->server_fd);
		return;
	}
	
	if (bind(this->server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) //attach socket to an IP address and port. (socket will listen on this specific address:port)
	{
		std::cerr << "Bind failed" << std::endl;
		return;
	}
	
	if (listen(this->server_fd, SOMAXCONN) < 0) //put socket into listening  mode (allow clients to connect)
	{
		std::cerr << "Listen failed" << std::endl;
		close (this->server_fd);
		return;
	}

	NewPoll.fd = this->server_fd;
	NewPoll.events = POLLIN; 					// set the event to POLLIN to read data
	NewPoll.revents = 0;						// set the revents to 0
	fds.push_back(NewPoll);						// add the server_fd to the pollfd vector

	std::cout << "Server listening on fd: " << this->server_fd << std::endl;
}
// // original void Server::sendLine(int fd, const std::string &msg)
// {
//     std::string out = msg + "\r\n";
//     const char *buf = out.c_str();
//     size_t len = out.size();
//     size_t sentTotal = 0;

//     while (sentTotal < len)
//     {
//         ssize_t n = ::send(fd, buf + sentTotal, len - sentTotal, 0);
//         if (n <= 0)
//             return; // optional: handle EAGAIN/EWOULDBLOCK/EINTR
//         sentTotal += static_cast<size_t>(n);
//     }
// }
void Server::sendLine(int fd, const std::string& msg)
{
    std::string out = msg + "\r\n";
    send(fd, out.c_str(), out.size(), 0);
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
// poll puts your process to sleep and wakes it when at least 1 fd becomes usable for the kind of I/O signal you care about (fds.events[i]) or when time runs out
// poll return value = total number of fds that have been selected (fds for which the revents member is non-zero), 0 indicates time-out (no fds selected), -1 some error or CRTL+C
void Server::RunServer()
{
	while (SignalReceived == false)
	{
 		int poll_count = poll(fds.data(), fds.size(), -1); // -1 = no time-out
		if (poll_count == -1)
		{
			std::cerr << "Poll failed " << std::endl;
			break ;  
		}

		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == this->server_fd)	// is it server socket? new client connecting
				{	
					AcceptClients();
					i++; // after AcceptClients(), fds vector might have changed, so we increment i to avoid skipping the next fd
					// for (; i < fds.size(); i++)
					// {
					// 	//Greeting(fds[i].fd);
					// }
					
					
				}
				else
				{
					//Greeting(fds[i].fd);								// client socket
					ReceiveData(fds[i].fd);
				}
					
			}

			if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))	// POLLHUP = hang up (client disconnected), POLLNVAL = invalid fd, POLLERR = error on the existing fd
			{
				std::cerr << "Error on fd: " << fds[i].fd << std::endl;
				RemoveClient(fds[i].fd);
			}
		}
	}
}

void Server::AcceptClients()
{
	int client_fd;
	struct pollfd NewPoll;
	// sockaddr_in client_addr;									// not needed if client address is not used
	// socklen_t client_len = sizeof(client_addr);				// not needed if client address is not used

	while (SignalReceived == false)
	{
		// client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
		client_fd = accept(server_fd, NULL, NULL);				// if client address is not needed, NULL can be passed as 2nd and 3rd argument
		if (client_fd < 0)
			break ;
		fcntl(client_fd, F_SETFL, O_NONBLOCK);					// set client socket to non-blocking mode
		NewPoll.fd = client_fd;
		NewPoll.events = POLLIN;
		NewPoll.revents = 0;
		fds.push_back(NewPoll);
		clients.push_back(Client(client_fd));					// not sure if vector of clients is needed
		std::cout << "New client connected, client_fd: " << clients.back().getFd() <<  " total clients: " << clients.size() << std::endl;
	}
}

size_t Server::findClientbyFd(int client_fd)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].getFd() == client_fd)
			return (i);
	}
	std::cerr << "Client not found, client_fd: " << client_fd << std::endl;
	return (-1);
}


/*
─────────────────────────────────────────────────┐
│              CLIENT BUFFER (string)            │
├────────────────────────────────────────────────┤
│                                                │
│  AppendToBuffer("NI")                          │
│  ↓                                             │
│  buffer = "NI"                                 │
│                                                │
│  AppendToBuffer("CK alice\r\n")                │
│  ↓                                             │
│  buffer = "NICK alice\r\n"                     │
│                                                │
│  ExtractCompleteCommands()                     │
│  ↓                                             │
│  Found: "NICK alice"                           │
│  buffer = "" (cleared)                         │
│  Returns: ["NICK alice"]                       │
│                                                │
└────────────────────────────────────────────────┘
*/

void Server::ReceiveData(int client_fd)
{

	char buffer[MAX_BUFFER_SIZE + 1]; // +1 for null-terminator
	// buffer[0] = '\0';
	int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0)
	{
		if (bytes_received == 0)
			std::cout << "Client disconnected, client_fd: " << client_fd << std::endl;
		
		else
			std::cerr << "Recv failed for client_fd: " << client_fd << std::endl;
		RemoveClient(client_fd);
		return;
	}
	
	buffer[bytes_received] = '\0'; // Null-terminate the received data
	std::cout << "Receiving data from client_fd: " << client_fd << std::endl;

	size_t client_index = findClientbyFd(client_fd);
	std::string str_buffer(buffer, bytes_received);			// std::string(buffer, bytes_received) Constructor for string
	clients[client_index].AppendToBuffer(str_buffer);

	std::vector<std::string> cmds =	clients[client_index].ExtractCompleteCommands();

for (size_t i = 0; i < cmds.size(); ++i)
	ProcessCommand(client_fd, cmds[i]);
}

void Server::HandlePass(int client_fd, const std::vector<std::string> &line)
{
    size_t client_index = findClientbyFd(client_fd);
    std::string pass = line[1];
    if (pass.empty())
    {
        std::cerr << "Client fd: " << client_fd << " provided empty password." << std::endl;
        return;
    }
    if (pass == this->password)
    {
        clients[client_index].setPassAccepted();
        std::cout << "Client fd: " << client_fd << " provided correct password." << std::endl;
    }
    else
        std::cout << "Client fd: " << client_fd << " provided incorrect password." << std::endl;
}

void Server::HandleNick(int client_fd, const std::vector<std::string> &line)
{
    size_t client_index = findClientbyFd(client_fd);
    std::string nickname = line[1];
    if (clients[client_index].getPassAccepted() == true)
    {
		for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i].getNickname() == nickname)
            {
                std::cerr << "Client fd: " << client_fd << " attempted to set duplicate nickname: " << nickname << std::endl;
                return;
            }
        }
        clients[client_index].setNickname(nickname);
        std::cout << "Client fd: " << client_fd << " set nickname to: " << nickname << std::endl;
        if (clients[client_index].getNickname().size() != 0 && clients[client_index].getUsername().size() != 0)
            clients[client_index].setRegistered();
    }
    else
        std::cerr << "Client fd: " << client_fd << " attempted to set nickname without passing authentication." << std::endl;
}
void Server::HandleUser(int client_fd, const std::vector<std::string> &line)
{
    size_t client_index = findClientbyFd(client_fd);
    std::string username = line[1];
    if (clients[client_index].getPassAccepted() == true)
    {
        clients[client_index].setUsername(username);
        std::cout << "Client fd: " << client_fd << " set username to: " << username << std::endl;
        if (clients[client_index].getNickname().size() != 0 && clients[client_index].getUsername().size() != 0)
            clients[client_index].setRegistered();
    }
    else
        std::cerr << "Client fd: " << client_fd << " attempted to set username without passing authentication." << std::endl;
}
/*void Server::HandleJoin(int client_fd, const std::vector<std::string> &line)
{
    size_t client_index = findClientbyFd(client_fd);
    std::string channel_name = line[1];
    
	if (clients[client_index].getIsRegistered() == true)
    {
        clients[client_index].setIsInChannel();
        std::cout << "Client fd: " << client_fd << " joined channel: " << channel_name << std::endl;
    }
    else
        std::cerr << "Client fd: " << client_fd << " attempted to join channel without completing registration." << std::endl;
}*/

void Server::HandleJoin(int client_fd, const std::vector<std::string>& line)
{
    int client_index = findClientbyFd(client_fd);
    if (client_index == -1)
        return;
    
    if (!clients[client_index].getIsRegistered())
    {
        sendLine(client_fd, "451 :You have not registered");
        return;
    }
    
    if (line.size() < 2)
    {
        sendLine(client_fd, "461 JOIN :Not enough parameters");
        return;
    }
    
    std::string channel_name = line[1];
    
    if (channel_name.empty() || channel_name[0] != '#')
    {
        sendLine(client_fd, "403 " + channel_name + " :No such channel");
        return;
    }
    
    // Create channel if doesn't exist
    if (!channelExists(channel_name))
        createChannel(channel_name);
    
    Channel* ch = getChannel(channel_name);
    if (!ch)
        return;
    
    // Check if already in channel
    if (ch->isUserInChannel(client_fd))
    {
        std::cout << "Already in channel" << std::endl;
        return;
    }
    
    // Add user to channel
    ch->addUser(&clients[client_index]);
    
    // First user becomes operator
    if (ch->getUserCount() == 1)
        ch->addOperator(client_fd);
    
    clients[client_index].setIsInChannel();
    
    std::string nick = clients[client_index].getNickname();
    std::string user = clients[client_index].getUsername();
    if (user.empty()) user = "user";
    
    // ✅ 1. Broadcast JOIN to ALL members (only once!)
    std::string joinMsg = ":" + nick + "!" + user + "@host JOIN " + channel_name;
    const std::map<std::string, Client*>& members = ch->getClients();
    for (std::map<std::string, Client*>::const_iterator it = members.begin();
         it != members.end(); ++it)
    {
        sendLine(it->second->getFd(), joinMsg);
    }
    // ✅ 2. Topic (only to joining user)
    sendLine(client_fd, "331 " + nick + " " + channel_name + " :No topic is set");
    
    // ✅ 3. Names list (only to joining user)
    std::string names = "353 " + nick + " = " + channel_name + " :";
    for (std::map<std::string, Client*>::const_iterator it = members.begin();
         it != members.end(); ++it)
    {
        if (ch->isOperator(it->second->getFd()))
            names += "@";
        names += it->first;
        names += " ";
    }
    sendLine(client_fd, names);
    
    // ✅ 4. End of names (only to joining user)
    sendLine(client_fd, "366 " + nick + " " + channel_name + " :End of /NAMES list");
    
    std::cout << "Client " << nick << " joined " << channel_name << std::endl;
}
   // line example: ["JOIN", "#chan"] or ["JOIN", "#chan,#chan2"]
 

void Server::HandlePrivMsg(int client_fd, const std::vector<std::string> &line)
{
    size_t client_index = findClientbyFd(client_fd);
	if (clients[client_index].getIsInChannel() == true)
	{
		std::string message = line[1];
		std::cout << "Client fd: " << client_fd << " sent message: " << message << std::endl;
		// Further message handling logic here
	}
	else
		std::cerr << "Client fd: " << client_fd << " attempted to send message without being in a channel." << std::endl;
}

void Server::HandleQuit(int client_fd)
{
	std::cout << "Client fd: " << client_fd << " is quitting." << std::endl;
	sendLine(client_fd, "\033[2J\033[H");  //Clear screen command");
	RemoveClient(client_fd);
}

// Channel management functions

bool Server::channelExists(const std::string& name) const
{
    return (channels.find(name) != channels.end());						// check if channel exists in the map
}

Channel* Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel>::iterator it = channels.find(name);		// find channel by name
	if (it != channels.end())
		return &(it->second);
	return NULL;
}

void Server::createChannel(const std::string& name) 						 // create a new channel
{
	if (!channelExists(name))												// check if channel already exists
	{
		channels.insert(std::make_pair(name, Channel(name)));				// insert new channel into map
		std::cout << "Channel created: " << name << std::endl;				
	}
	else
	{
		std::cerr << "Channel already exists: " << name << std::endl;
	}
}

void Server::removeChannel(const std::string& name)
{
	std::map<std::string, Channel>::iterator it = channels.find(name);			// find channel by name
	if (it != channels.end())													// if channel found, erase it from map
	{
		channels.erase(it);
		std::cout << "Channel removed: " << name << std::endl;
	}
	else
	{
		std::cerr << "Channel not found: " << name << std::endl;
	}
}

// End of channel management functions

void Server::SendMessage(int client_fd, const std::vector<std::string> &line)
{
	
	 size_t client_index = findClientbyFd(client_fd);
	 if(clients[client_index].getIsInChannel() == true)
	 {
		 for(size_t i = 1; i < line.size(); i++)
		 	std::cout<< line[i] << std::endl;
	 }

	 else
	 {
		 std::cerr << "Client fd: " << client_fd << " attempted to send message without being in a channel." << std::endl;
		 return;
	 }
}

std::vector<std::string> ParseCommand(const std::string& line)
{
    std::vector<std::string> tokens;
    std::string current;
    bool in_trailing = false;
    
    for (size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];
        
        // Trailing parameter (starts with :)
        if (c == ':' && !in_trailing && (i == 0 || line[i-1] == ' '))
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

void Server::ProcessCommand(int client_fd, const std::string& line)
{
    if (line.empty())
        return;
    
    // ✅ HIER parsed du den String in Tokens
    std::vector<std::string> tokens = ParseCommand(line);
    
    if (tokens.empty())
        return;
    
    int client_index = findClientbyFd(client_fd);
    if (client_index == -1)
        return;
    
    std::string command = tokens[0];
    
    // Convert to uppercase
    for (size_t i = 0; i < command.size(); i++)
        command[i] = std::toupper(command[i]);
    
    std::cout << "[" << client_fd << "] " << command << std::endl;
    
    // Route commands
    if (command == "CAP")
    {
        sendLine(client_fd, "CAP * LS :");
    }
    else if (command == "PING")
    {
        if (tokens.size() > 1)
            sendLine(client_fd, "PONG ircserv :" + tokens[1]);
        else
            sendLine(client_fd, "PONG ircserv");
    }
    else if (command == "PASS")
        HandlePass(client_fd, tokens);
    else if (command == "NICK")
        HandleNick(client_fd, tokens);
    else if (command == "USER")
        HandleUser(client_fd, tokens);
    else if (command == "JOIN")
        HandleJoin(client_fd, tokens);
    else if (command == "PRIVMSG")
        HandlePrivMsg(client_fd, tokens);
    else if (command == "QUIT")
        HandleQuit(client_fd);
    else if (clients[client_index].getIsInChannel())
        SendMessage(client_fd, tokens);
    else
    {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void Server::RemoveClient(int client_fd)    // We have to QUIT twice to quit once !!!! ERROR !!!! 
{
    /*  pollfd entfernen */
    for (size_t i = 0; i < fds.size(); ++i)
    {
        if (fds[i].fd == client_fd)
        {
            fds.erase(fds.begin() + i);
            std::cout << "Pollfd removed, client_fd: "
                      << client_fd
                      << " total fds: "
                      << fds.size()
                      << std::endl;
            break;
        }
    }

    /*  remove Client-Objekt  */
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (clients[i].getFd() == client_fd)
        {
            clients.erase(clients.begin() + i);
            std::cout << "Client removed, client_fd: "
                      << client_fd
                      << " total clients: "
                      << clients.size()
                      << std::endl;
            break;
        }
    }

    /*  close Socket */
    
	close(client_fd);
	
}


// void Server::RemoveClient(int client_fd)
// {
// 	close (client_fd);
//     for (size_t i = 0; i < fds.size(); i++)
//     {
//         if (fds[i].fd == client_fd)
//         {
//             fds.erase(fds.begin() + i);
// 			std::cout << "Pollfd removed, client_fd: " << client_fd << " total fds: " << fds.size() << std::endl;
            
// 			if (clients[i-1].getFd() == client_fd)
// 			{
// 				clients.erase(clients.begin() + i-1);
// 				std::cout << "Client removed, client_fd: " << client_fd << " total clients: " << clients.size() << std::endl;
// 				break;
// 			}
			
//         }
//     }

	// for (size_t i = 0; i < clients.size(); i++)
	// {
	// 	if (clients[i].getFd() == client_fd)
	// 	{
	// 		clients.erase(clients.begin() + i);
	// 		std::cout << "Client removed, client_fd: " << client_fd << " total clients: " << clients.size() << std::endl;
	// 		break;
	// 	}
	// }
// }

void Server::ClearClients()
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        int fd = clients[i].getFd();
        if (fd >= 0)
            close(fd);
    }
    clients.clear();
}

const char *Server::InvalidInput::what() const throw()
{
	return (RED "Invalid input: " R BLU "Port argument can only consist digits 0-9.\n (Note: zero as first digit is also not allowed)" R);
}

const char *Server::InvalidArgsAmount::what() const throw()
{
	return (RED "Invalid arguments amount: "  R BLU "Usage: ./ft_irc <port> <password>" R);
}

const char *Server::InvalidRange::what()  const throw()
{
	return (RED "invalid port number: " R BLU "try between 1024 - 65535" R);
}

const char *Server::PassLengh::what() const throw()
{
	return ( RED "Password should be between 3 to 16 characters" R);
} 

const char *Server::PassAsciOnly::what() const throw()
{
		return( RED "Password can only consist ASCII characters" R);
}