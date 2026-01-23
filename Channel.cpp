#include "Channel.hpp"

Channel::Channel(const std::string& name)
    : name(name), topic(""), password(""), inviteOnly(false), topicRestricted(false), userLimit(0)
{
    std::cout << "Channel " << name << " created." << std::endl;
}

Channel::Channel(const Channel& other)
{
    *this = other;
}

Channel& Channel::operator=(const Channel& other)
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
    std::cout << "Channel " << name << " destroyed." << std::endl;
}


void Channel::addUser(int client_fd)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == client_fd)
            return ;                     // User already in channel
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
            break ;
        }
    }

    for (size_t i = 0; i < invitedUsers.size(); i++)
    {
        if (invitedUsers [i] == client_fd)
        {
            invitedUsers.erase(invitedUsers.begin() + i);
            break ;
        }
    }
}

void Channel::addOperator(int client_fd)
{
    for (size_t i = 0; i < operators.size(); i++)
    {
        if (operators[i] == client_fd)
            return ;                     // User already an operator
    }
    operators.push_back(client_fd);
}



void Channel::removeOperator(int client_fd)
{

}

bool Channel::isUserInChannel(int client_fd) const
{

}

bool Channel::isOperator(int client_fd) const
{

}

        // void addInvitedUser(int client_fd);
        // bool isUserInvited(int client_fd) const;

        // std::string getName() const;
        // std::string getTopic() const;

        // void setTopic(const std::string& newTopic);
        // void setPassword(const std::string& newPassword);
        // void setInviteOnly(bool inviteOnly);
        // void setTopicRestricted(bool topicRestricted);
        // void setUserLimit(int limit);