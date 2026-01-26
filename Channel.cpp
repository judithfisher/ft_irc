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
