/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:44:51 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/31 23:39:29 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib>		//for atoi()
#include <vector>		//for vector
#include <sys/socket.h> //for socket()
#include <sys/types.h>	//for socket()
#include <netinet/in.h> //for sockaddr_in
#include <fcntl.h>		//for fcntl()
#include <unistd.h>		//for close()
#include <arpa/inet.h>	//for inet_ntoa()
#include <poll.h>		//for poll()
#include <csignal>		//for signal()
#include <algorithm>	//for find()
#include <sstream>		//for istringstream
#include <cctype>		//for isdigit() isascii()
#include <cstring>		//for strlen()
#include <map>			//for map  , channels will be organiyzed in map with channel name as key and Channel object as value
// my includes
#include "Channel.hpp" //for Channel class
#include "Client.hpp"  //for Client class

#define R "\033[0m"
#define RED "\033[38;2;239;68;68m"
#define GRN "\033[38;2;34;197;94m"
#define BLU "\033[38;2;59;130;246m"
#define YEL "\033[38;2;234;179;8m"
#define PRP "\033[38;2;168;85;247m"
#define ORN "\033[38;2;255;165;0m"
#define BOLD "\033[1m"

class Server
{
private:
	int port;
	int server_fd;

	std::string password;
	std::vector<Client> clients;			 // to keep track of connected clients + to manage their requests
	std::vector<struct pollfd> fds;			 // vector of pollfd, to monitor multiple file descriptors
	std::map<std::string, Channel> channels; // map of channel name to Channel object
public:
	Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	int getServerfd();
	std::vector<Client> getClientvector();

	void setport(int port);
	void setpassword(std::string password);

	static void SignalHandler(int signum);

	void InitServerSocket();
	static void sendLine(int fd, const std::string &msg);
	void RunServer();
	void AcceptClients();
	void Greeting(int client_fd);
	int findClientbyFd(int client_fd);
	void ReceiveData(int client_fd);

	std::vector<std::string> ParseCommand(const std::string &line);
	void ProcessCommand(int client_fd, const std::string &line);
	void HandlePass(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleNick(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleUser(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleJoin(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandlePrivMsg(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleTopic(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleInvite(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleKick(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleQuit(int client_fd, int client_index);
	void SendMessage(int client_fd, const std::vector<std::string> &line, int client_index);
	void HandleMode(int client_fd, const std::vector<std::string> &line, int client_index);

	void RemoveClient(int client_fd);
	void ClearClients();

	// Channel management
	bool channelExists(const std::string &name) const; // check if channel exists
	Channel *getChannel(const std::string &name);	   // get pointer to channel by name
	void createChannel(const std::string &name); // create a new channel
	void removeChannel(const std::string &name); // remove channel by name
	// End of channel management
	static bool SignalReceived; // part of class server, not individual objects --> static

	// For try catch blockm
	class InvalidInput : public std::exception
	{
	public:
		const char *what() const throw();
	};

	class InvalidArgsAmount : public std::exception
	{
	public:
		const char *what() const throw();
	};

	class InvalidRange : public std::exception
	{
	public:
		const char *what() const throw();
	};
	class PassLengh : public std::exception
	{
	public:
		const char *what() const throw();
	};

	class PassAsciOnly : public std::exception
	{
		const char *what() const throw();
	};
};

#endif

/*
The <netinet/in.h> header defines the sockaddr_in structure that includes at least the following members:

sa_family_t     sin_family   AF_INET
in_port_t       sin_port     Port number
struct in_addr  sin_addr     IP address
*/
