#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"

class Channel
{
    public:
        Channel(const std::string& name);
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);
        ~Channel();

        void addUser(int client_fd);
        void removeUser(int client_fd);
        void addOperator(int client_fd);
        void removeOperator(int client_fd);
        bool isUserInChannel(int client_fd) const;
        bool isOperator(int client_fd) const;

        void addInvitedUser(int client_fd);
        bool isUserInvited(int client_fd) const;

        std::string getName() const;
        std::string getTopic() const;

        void setTopic(const std::string& newTopic);
        void setPassword(const std::string& newPassword);
        void setInviteOnly(bool inviteOnly);
        void setTopicRestricted(bool topicRestricted);
        void setUserLimit(int limit);

    private:
        std::vector<int> users;
        std::vector<int> operators;
        std::vector<int> invitedUsers;

        std::string name;
        std::string topic;
        std::string password;

        bool inviteOnly;
        bool topicRestricted;
        int userLimit;
};

#endif