/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:41 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:01:19 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <set>

#include "User.hpp"


/* 
    CLASS FOR MODELING/DTO (DATA TRANSFER OBJECT),
    WHICH EXISTS MORE FOR THE PURPOSE OF STORING DATA
    RATHER THAN EXECUTING AND/OR HAVING COMPLEX LOGIC    
*/
class Channel
{
private:
    enum mode
    {
        INVITE_ONLY,
        RESTRICTED_TOPIC,
        REQUIRES_PASSWORD,
        LIMITED
    };

    std::vector<User*> _users;
    std::set<int> _operators;
    std::set<int> _invitedUsers;
    std::string topic;
    std::string _channelName;
    std::string _password;
    int _userLimit;
    
    bool _inviteOnly;
    bool _restrictedTopic;
    bool _requiresPassword;
    bool _limitedUsers;
    
public:
    Channel(std::string ChannelName);
    ~Channel();

    void AddUser(User *user);
    void removeUser(User *user);
    void removeUser(int fd);
    bool isUserInChannel(int fd) const;
    bool isOperator(int fd) const;
    void promoteToOperator(const std::string& nickname);
    void demoteOperator(const std::string& nickname);
    
    std::vector<User*> getUsers() const;
    std::string getChannelName() const;
    std::string getChannelTopic() const;
    std::string getAllUserString() const;
    std::string getPassword() const;
    size_t getUserLimit() const;

    void setInviteOnly(bool enabled);
    void setRestrictedTopic(bool enabled);
    void setRequiresPassword(std::string password);
    void removePassword();
    void setUserLimit(int limit);
    void removeUserLimit();

    bool isInviteOnly() const;
    bool isRestrictedTopic() const;
    bool hasPassword() const;
    bool hasUserLimit() const;
    bool isUserInvited(User *user) const;

    void setChannelTopic(std::string topic);
    void setChannelPassword(std::string password);
    void setChannelLimit(int limit);
    void inviteUser(User *user);

    std::string getModeParameters(bool showPassword) const;
    std::string getModeString() const;

    void broadcastMessageTemp(const std::string& message, int senderFd);
    
};
