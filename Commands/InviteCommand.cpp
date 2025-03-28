/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:05:45 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/26 14:27:33 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

InviteCommand::InviteCommand()
{}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(MessageContent messageContent, int clientFd)
{
	User *user = UserService::getInstance().findUserByFd(clientFd);
	if (!user || !user->isAuthenticated())  // Check if user is authenticated
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTREGISTERED, user->getNick());
		return;
	}
	if (messageContent.tokens.size() < 3) // Check if enough parameters are given
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NEEDMOREPARAMS, "INVITE");
		return;
	}

	std::string channelName = messageContent.tokens[2];
	std::string targetNick = messageContent.tokens[1];

	Channel *channel = ChannelService::getInstance().findChannel(channelName);
	if (!channel) // Channel does not exist
	{
		// _userService->sendMessage(clientFd, ERR_NOSUCHCHANNEL(user->getNick(), channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
		return;
	}

	User *targetUser = UserService::getInstance().findUserByNickname(targetNick);
	if (!targetUser) // Target user does not exist
	{
		// _userService->sendMessage(clientFd, ERR_NOSUCHNICK(user->getNick(), targetNick));
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHNICK, user->getNick(), targetNick);
		return;
	}

	if (!channel->isUserInChannel(clientFd)) // Inviter must be on the channel
	{
		// _userService->sendMessage(clientFd, ERR_NOTONCHANNEL(user->getNick(), channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTONCHANNEL, user->getNick(), channelName);
		return;
	}

	if (channel->isUserInvited(targetUser) || channel->isUserInChannel(targetUser->getFd())) // Check if already invited
	{
		// _userService->sendMessage(clientFd, ERR_USERONCHANNEL(user->getNick(), targetNick, channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_USERONCHANNEL, user->getNick(), channelName);
		return;
	}

	if (!channel->isOperator(clientFd)) // Check if the user has operator privileges
	{
		// _userService->sendMessage(clientFd, ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
		return;
	}

	// Invite the user
	channel->inviteUser(targetUser);
	UserService::getInstance().sendMessage(targetUser->getFd(),
    ":" + user->getNick() + "!~" + user->getUser() + "@host" + 
    " INVITE " + targetNick + " " + channelName + "\r\n");
	

	std::cout << "[DEBUG]: User " << targetUser->getNick() << " invited to channel " << channel->getChannelName() << std::endl;
}

