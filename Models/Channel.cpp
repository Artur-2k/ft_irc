/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:26 by davi              #+#    #+#             */
/*   Updated: 2025/03/25 18:30:01 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sstream>
#include <sys/socket.h>

/* 
    CLASSE PARA MODELAGEM/DTO (DATA TRANSFER OBJECT),
    ONDE EXISTE MAIS COM O OBJETIVO DE GUARDAR DADOS
    DO QUE PROPRIAMENTE EXECUTAR E/OU TER LOGICAS COMPLEXAS    
*/



/* 
    SETA NOME DO CANAL AO CONSTRUIR TAL OBJETO
    E INICIA O TOPICO DO CANAL COMO DEFAULT,
    PODENDO DEPOIS SER MODIFICADO UTILIZANDO
    UM SIMPLES SETTER QUE NAO EXISTE POR ENQUANTO,
    POIS NAO EXISTE TAL COMANDO TBM
 */
Channel::Channel(std::string ChannelName)
    : topic("Default topic"), _channelName(ChannelName), _password(""), _userLimit(0), _inviteOnly(false), _restrictedTopic(false), _requiresPassword(false), _limitedUsers(false)
{
}

Channel::~Channel()
{
}

/* 
    ADICIONA PONTEIRO DE USUARIO AO VETOR
    DE USUARIOS QUE FAZEM PARTE DO CANAL

    OBS: TALVEZ MODIFICAR PARA REFERENCIA
    O ARG DESSE METODO, PARA CODIGO MAIS SAFE

    MIGRAR PARA CHANNELSERVICE?
 */
void Channel::AddUser(User* user)
{
    _users.push_back(user);
    if (_users.size() == 1)
        _operators.insert(user->getFd());
}

// OPERADOR PODE SER REMOVIDO?
void Channel::removeUser(User* user)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == user->getFd())
        {
            _users.erase(_users.begin() + i);
            break;
        }
    }
}

void Channel::removeUser(int fd)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == fd)
        {
            _users.erase(_users.begin() + i);
            break;
        }
    }
}

//JA EXISTE EM CHANNELSERVICE
bool Channel::isUserInChannel(int fd) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == fd)
            return true;
    }
    return false;
}

bool Channel::isOperator(int fd) const
{
    return _operators.find(fd) != _operators.end();
}

void Channel::promoteToOperator(const std::string& nickname)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getNick() == nickname)
        {
            _operators.insert(_users[i]->getFd());
            return;
        }
    }
}

void Channel::demoteOperator(const std::string& nickname)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getNick() == nickname)
        {
            _operators.erase(_users[i]->getFd());
            return;
        }
    }
}

std::vector<User*> Channel::getUsers() const
{
    return this->_users;
}

std::string Channel::getChannelName() const
{
    return this->_channelName;
}

std::string Channel::getChannelTopic() const
{
    return this->topic;
}


/* 
    IMPLEMENTADO PARA SER UTILIZADO NO SERVER MESSAGES
    PARA FACILMENTE RETORNAR UMA STRING COM TODOS USERS
    DE DETERMINANDO CANAL, DEIXANDO O SERVER MESSAGES
    UM POUCO MAIS CLEAN, SENDO QUE JA ESTA MUITO FEIO
    AQUELA PARTE DO CODIGO
 */
std::string Channel::getAllUserString() const
{
    std::string result;
    
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (i > 0)
            result.append(" "); 

        if (isOperator(_users[i]->getFd()))
            result.append("@");
        
        result.append(_users[i]->getNick());
    }
    
    return result;
}


std::string Channel::getPassword() const
{
    return this->_password;
}

size_t Channel::getUserLimit() const
{
    return this->_userLimit;
}

void Channel::setInviteOnly(bool enabled)
{
    this->_inviteOnly = enabled;
}

void Channel::setRestrictedTopic(bool enabled)
{
    this->_restrictedTopic = enabled;
}

void Channel::setRequiresPassword(std::string password)
{
    this->_password = password;
    this->_requiresPassword = true;
}

void Channel::removePassword()
{
    this->_password = "";
    this->_requiresPassword = false;
}

void Channel::setUserLimit(int limit)
{
    this->_userLimit = limit;
    this->_limitedUsers = true;
}

void Channel::removeUserLimit()
{
    this->_userLimit = 0;
    this->_limitedUsers = false;
}

bool Channel::isInviteOnly() const
{
    return this->_inviteOnly;
}

bool Channel::isRestrictedTopic() const
{
    return this->_restrictedTopic;
}

bool Channel::hasPassword() const
{
    return this->_requiresPassword;
}

bool Channel::hasUserLimit() const
{
    return this->_limitedUsers;
}



void Channel::setChannelPassword(std::string password)
{
    this->_password = password;
}

void Channel::setChannelLimit(int limit)
{
    this->_userLimit = limit;
}

void Channel::inviteUser(User* user)
{
    _invitedUsers.insert(user->getFd());
}

void Channel::setChannelTopic(std::string topic)
{
    this->topic = topic;
}

bool Channel::isUserInvited(User* user) const
{
    return _invitedUsers.find(user->getFd()) != _invitedUsers.end();
}

std::string Channel::getModeParameters(bool showPassword) const
{
    std::stringstream params;

    if (showPassword && !_password.empty())  // Só mostra se permitido
        params << _password << " ";
    if (_userLimit > 0)
        params << _userLimit;

    return params.str();
}

std::string Channel::getModeString() const
{
    std::string modes = "+";

    if (_inviteOnly)
        modes += "i";
    if (_restrictedTopic)
        modes += "t";
    if (!_password.empty())
        modes += "k";
    if (_userLimit > 0)
        modes += "l";

    return (modes.size() > 1) ? modes : "";
}


// TEMPORARIA - MIGRAR PARA SERVER MESSAGES???
void Channel::broadcastMessageTemp(const std::string& message, int senderFd)
{
    for (size_t i = 0; i < _users.size(); ++i)
    {
        User* user = _users[i];
        if (senderFd == 0 || user->getFd() != senderFd)
        {
            send(user->getFd(), message.c_str(), message.size(), 0);
        }
    }
}