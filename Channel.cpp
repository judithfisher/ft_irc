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