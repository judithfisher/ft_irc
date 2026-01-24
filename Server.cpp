/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judith <judith@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:23:52 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/23 17:16:38 by judith           ###   ########.fr       */
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
void Server::sendLine(int fd, const std::string &msg)
{
    std::string out = msg + "\r\n";
    const char *buf = out.c_str();
    size_t len = out.size();
    size_t sentTotal = 0;

    while (sentTotal < len)
    {
        ssize_t n = ::send(fd, buf + sentTotal, len - sentTotal, 0);
        if (n <= 0)
            return; // optional: handle EAGAIN/EWOULDBLOCK/EINTR
        sentTotal += static_cast<size_t>(n);
    }
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
					Greeting(fds[i].fd);								// client socket
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
	
	std::vector<std::string> line = clients[client_index].ExtractCompleteCommands(str_buffer);  // extract complete commands from client buffer
	if(line.empty())
		return;
	ProcessCommand(client_fd, line);		// implement command processing logic here
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

void Server::HandleJoin(int client_fd, const std::vector<std::string> &line)			// corrected version
{
    size_t client_index = findClientbyFd(client_fd);									// find client index by fd
    std::string channel_name = line[1];													// get channel name from command arguments
																						// check if client is registered		
    if (!clients[client_index].getIsRegistered())										
    {
        std::cerr << "Client fd: " << client_fd							
                  << " attempted to join channel without completing registration."
                  << std::endl;
        return;
    }

    // create Channel if it doesn't exist
    if (!channelExists(channel_name))
        createChannel(channel_name);

    // get pointer to Channel object
    Channel* ch = getChannel(channel_name);
    if (!ch)
        return;

    // 	add client to channel
    ch->addUser(&clients[client_index]);

    // mark client as being in a channel	
    clients[client_index].setIsInChannel();

    std::cout << "Client fd: " << client_fd
              << " joined channel: " << channel_name << std::endl;
	sendLine(client_fd, ":" + clients[client_index].getNickname() + "!user@host JOIN " + channel_name);

}

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

void Server::ProcessCommand(int client_fd, const std::vector<std::string> &line)
{
	size_t client_index = findClientbyFd(client_fd);
	std::string command = line[0];
	
	for (size_t i = 0; i < command.size(); i++)
		command[i] = toupper(command[i]);
	

	if (command == "PASS")
		HandlePass(client_fd, line);
	else if (command == "NICK" )
		HandleNick(client_fd, line);
	else if (command == "USER" )
		HandleUser(client_fd, line);
	else if (command == "JOIN")
		HandleJoin(client_fd, line);
	else if (command == "PRIVMSG")
		HandlePrivMsg(client_fd, line);
	else if (command == "QUIT")
		HandleQuit(client_fd);		//later implement HandleQuit(client_fd);
	else if (clients[client_index].getIsInChannel() == true)
		SendMessage(client_fd, line);
	else
		std::cerr << "Unknown command from client_fd: " << client_fd << " Command: " << command << std::endl;
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