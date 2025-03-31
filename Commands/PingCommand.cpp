/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:51:17 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/31 22:54:09 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PingCommand.hpp"

PingCommand::PingCommand(/* args */)
{
}

PingCommand::~PingCommand()
{
}

std::string PingCommand::getToken() const
{
    return this->_token;
}

void PingCommand::execute(MessageContent messageContent, int fd)
{
    if (UserService::getInstance().findUserByFd(fd)->getStatus() != User::AUTHENTICATED)
        return ;
        
    if (messageContent.tokens.size() == 2)
    {
        std::string svName = Server::getInstance().getServerName();
        std::ostringstream ss;
        ss << ":" << svName << " PONG " << svName << " " << messageContent.tokens[1] << "\r\n";
        send(fd, ss.str().c_str(), ss.str().size(), 0);
    }
    else
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS);
    return ;
}