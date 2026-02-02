#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(const std::string &name): name(name), topic(""), password(""), inviteOnly(false), topicRestricted(false), userLimit(0)
{
}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->name = other.name;
		this->topic = other.topic;
		this->password = other.password;
		this->inviteOnly = other.inviteOnly;
		this->topicRestricted = other.topicRestricted;
		this->userLimit = other.userLimit;
	}
	return (*this);
}

Channel::~Channel()
{
}

void Channel::addUser(int client_fd, const std::string &client_nick)
{
	channel_clients.insert(std::make_pair(client_nick, client_fd));

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] == client_fd)
			return ;  // Already in channel
	}
	users.push_back(client_fd);
}

void Channel::updateUserNick(int client_fd, const std::string &new_nick)
{
	for (std::map<std::string, int>::iterator it = channel_clients.begin(); it != channel_clients.end(); it++)
	{
		if (it->second == client_fd)
		{
			channel_clients.erase(it);
			break;
		}
	}
	channel_clients.insert(std::make_pair(new_nick, client_fd));
}

void Channel::removeUser(int client_fd)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] == client_fd)
		{
			users.erase(users.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < invitedUsers.size(); i++)
	{
		if (invitedUsers[i] == client_fd)
		{
			invitedUsers.erase(invitedUsers.begin() + i);
			break;
		}
	}

	for (std::map<std::string, int>::iterator it = channel_clients.begin();
		it != channel_clients.end(); ++it)
	{
		if (it->second == client_fd)
		{
			channel_clients.erase(it);
			break;
		}
	}

	/* check are users still in channel and is there no operator 
	then we need to sign up new one which will be first element of vector of users*/
	if (!users.empty() && operators.empty()) 
	{
		int new_oper = users[0];
		addOperator(new_oper);
		broadcast("New channel operator assigned"); /* Does it have to be specified who new became an operator? */
	}
}

void Channel::addOperator(int client_fd)
{
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == client_fd)
			return; // User already an operator
	}
	operators.push_back(client_fd);
}

void Channel::removeOperator(int client_fd)
{
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == client_fd)
		{
			operators.erase(operators.begin() + i);

			break;
		}
	}
}

bool Channel::isUserInChannel(int client_fd) const
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] == client_fd)
			return true;
	}
	return false;
}

bool Channel::isOperator(int client_fd) const
{
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == client_fd)
			return true;
	}
	return false;
}

size_t Channel::getFirstUser() const
{
	return(this->users[0]);
}

size_t Channel::getOperatorsSize() const
{
    size_t opr_size = this->operators.size();
	return (opr_size);
}

std::string Channel::getName() const
{
	return(this->name);
}


// broadcast to all clients
void Channel::broadcast(const std::string &msg)
{
	for (std::map<std::string, int>::iterator it = channel_clients.begin(); it != channel_clients.end(); ++it)
		Server::sendLine(it->second, msg);
}

const std::map<std::string, int> &Channel::getClients() const
{
	return (channel_clients);
}

size_t Channel::getUserCount() const
{
    size_t user_size = this->users.size();
	return (user_size);
}

std::string Channel::getTopic() const
{
	return (this->topic);
}

bool Channel::getTopicRestricted() const
{
	return (this->topicRestricted);
}

void Channel::setTopic(const std::string &newTopic)
{
	this->topic = newTopic;
}

void Channel::setTopicRestricted(bool topicRestricted)
{
	this->topicRestricted = topicRestricted;
}

//MODE

void Channel::setInviteOnly(bool inviteOnly) 
{
	// std::cout << "[DEBUG] setInviteOnly called with: " << inviteOnly << std::endl;
	this->inviteOnly = inviteOnly;
}

bool Channel::isInviteOnly() const 
{
	// std::cout << "[DEBUG] isInviteOnly returns: " << this->inviteOnly << std::endl;
	return this->inviteOnly;
}

void Channel::setPassword(const std::string &password) 
{ 
    this->password = password; 
}

void Channel::removePassword() 
{ 
	this->password.clear(); 
}

bool Channel::hasPassword() const 
{ 
	return !this->password.empty(); 
}

std::string Channel::getPassword() const 
{ 
	return this->password; 
}

void Channel::setUserLimit(int limit) 
{ 
	this->userLimit = limit; 
}

void Channel::removeUserLimit() 
{ 
	this->userLimit = 0; 
}

int Channel::getUserLimit() const 
{ 
	return this->userLimit; 
}

void Channel::addInvitedUser(int client_fd) 
{
    for (size_t i = 0; i < invitedUsers.size(); i++)
        if (invitedUsers[i] == client_fd)
            return;
    invitedUsers.push_back(client_fd);
}

void Channel::removeInvitedUser(int client_fd) 
{
	for (size_t i = 0; i < invitedUsers.size(); i++)
	{
		if (invitedUsers[i] == client_fd)
		{
			invitedUsers.erase(invitedUsers.begin() + i);
			break;
		}
	}
}

bool Channel::isUserInvited(int client_fd) const 
{
    for (size_t i = 0; i < invitedUsers.size(); i++)
        if (invitedUsers[i] == client_fd)
            return true;
    return false;
}