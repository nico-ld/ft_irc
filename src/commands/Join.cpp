/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:27:18 by nico              #+#    #+#             */
/*   Updated: 2026/09/02 15:19:56 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "../../includes/Parser.hpp"

/* > Reply to the client, send information about channel joined */
static void joinReply(Server *server, User *client, Channel &channel, Parser &parser) {
	// Echoes the command
	server->broadcast(channel,  client->getPrefix() + " " + parser.getRawString());

	// Send topic information
	server->topic(channel, client);

	// Send user name list in channel
	std::string messagePrefix = "= " + channel.getName() + " :";
	std::string message;
	std::map<int, User*> userList = channel.getMembers();
	for (std::map<int, User*>::iterator it = userList.begin(); it != userList.end(); ++it) {
		if (message.size() < 400) {
			// If user is operator, add '@'
			if (channel.isOperator(it->second->getFd())) {
				message.append("@");
			}

			// Add user name
			message.append(it->second->getNickname());
			message.append(" ");
		} else {
			server->sendReply(*client, RPL_NAMREPLY, messagePrefix + message);
			message.clear();
		}
	}
	if (!message.empty()) {
		server->sendReply(*client, RPL_NAMREPLY, messagePrefix + message);
	}

	// End of name list
	server->sendReply(*client, RPL_ENDOFNAMES, channel.getName() + " :End of user name list");
}

static bool userCantJoin(Server *server, Channel &channel, User *client) {
	// Check if user has joined too many channels or not
	if (client->getJoinedChannels().size() > 15) {
		server->dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", Joined too many channel");
		server->sendReply(*client, ERR_TOOMANYCHANNELS, "You have joined too many channel");
		return (true);
	}

	// Check if user is already on the channel
	if (channel.isMember(client->getFd())) {
		server->dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ": Trying to join a channel already joined");
		server->sendReply(*client, ERR_USERONCHANNEL, "You already are on " + channel.getName());
		return (true);
	}
	
	// Check channel is on invite only (+i)
	if (channel.isInviteOnly() && !channel.isInvited(client->getFd())) {
		server->dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", Channel is on invite only mode");
		server->sendReply(*client, ERR_INVITEONLYCHAN, "Channel is on invite only mode (+i)");
		return (true);
	}

	// Check if the channel get a user limit and if there is too many user for that limit (+l)
	if (channel.getUserLimit() != -1 && channel.getMemberCount() >= channel.getUserLimit()) {
		server->dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", Channel is full");
		server->sendReply(*client, ERR_CHANNELISFULL, "Channel is full, you cannot join it (+l)");
		return (true);
	}
	return (false);
}

void Server::join(std::vector<Channel> &listChannel, User *client, Parser &parser) {
	// Parse every channels
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		// Check if channel name is correct
		if (!parser.checkChannelName(getChan->getName())) {
			dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", Invalid channel name");
			sendReply(*client, ERR_NOSUCHCHANNEL, "Invalid channel name");
			return ;
		}

		// Check if channel already exist
		std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());

		// === CHANNEL EXIST ===
		if (it != _channels.end()) {
			// Check if user can join the channel
			if (userCantJoin(this, it->second, client))
				return ;

			// Check if channel need a key to be joined (+k)
			if (it->second.getKey().size() > 0) {
				dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", This channel need a key");
				sendReply(*client, ERR_BADCHANNELKEY, "This channel need a key to join it");
				return ;
			}

			// If every guard is OK, join the channel
			it->second.addMember(client);
			client->joinChannel(it->second.getName());
			joinReply(this, client, it->second, parser);
		}


		// === CREATE CHANNEL ===
		else {
			// Create new channel
			Channel channel(getChan->getName());

			// Init new channel
			channel.addMember(client); // Add the new member
			channel.addOperator(client); // By default the first user is a channel operator
			_channels.insert(std::make_pair(getChan->getName(), channel)); // Add channel to channel list
			client->joinChannel(getChan->getName());

			// Send message
			joinReply(this, client, channel, parser);

			// Update dashboard
			dash->increaseInfo(dash->getSectionByIndex(1), LEFT, 0);
			dash->log(INFO, "New channel : " + channel.getName() + " have been created");
			continue;
		}
	}
}

void Server::join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client, Parser &parser) {
	size_t i = 0;

	// Parse every channels
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		// Check if channel name is correct
		if (!parser.checkChannelName(getChan->getName())) {
			dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", Invalid channel name");
			sendReply(*client, ERR_NOSUCHCHANNEL, "Invalid channel name");
			return ;
		}

		// Check if there still is somes key in the given list
		if (i < listKey.size()) {
			// Check if channel alread exist
			std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());

			// === CHANNEL EXIST ===
			if (it != _channels.end()) {
				// Check if user can join the server
				if (userCantJoin(this, it->second, client))
					return ;

				// Check if the key is valid
				if (it->second.getKey() != listKey[i]) {
					dash->log(WARNING, "Fd : " + toStr(client->getFd()) + ", Invalid channel key");
					sendReply(*client, ERR_BADCHANNELKEY, "Invalid channel key");
					return ;
				}

				// If every guard are OK, user can join the channel
				it->second.addMember(client);
				client->joinChannel(it->second.getName());
				joinReply(this, client, it->second, parser);
			}

			// === CREATE CHANNEL ===
			else {
				// Create channel
				Channel channel(getChan->getName(), listKey[i]);

				// Init channel
				channel.addMember(client); // Add the first member
				channel.addOperator(client); // By default the first member is a channel operator
				_channels.insert(std::make_pair(getChan->getName(), channel)); // Insert channel in channel list
				client->joinChannel(getChan->getName());

				// Send message
				joinReply(this, client, channel, parser);

				// Update dashboard
				dash->increaseInfo(dash->getSectionByIndex(1), LEFT, 0);
				dash->log(INFO, "New channel : " + channel.getName() + " have been created");
				continue;
			}	
		}

		// If not anymore keys, continue on the other JOIN
		else {
			std::vector<Channel> restOfListChannel(getChan, listChannel.end());
			join(restOfListChannel, client, parser);
			return ;
		}
		++i;
	}
}
