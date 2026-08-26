#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::join(std::vector<Channel> &listChannel, User *client, Parser &parser) {
	// Parse every channels
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		// Check if user has joined too many channels or not
		if (client->getJoinedChannels().size() > 15) {
			sendReply(*client, ERR_TOOMANYCHANNELS, "You have joined too many channel");
			throw std::runtime_error("[LOG] User joined too many channels");
		}

		// Check if channel name is correct
		if (!parser.checkChannelName(getChan->getName())) {
			sendReply(*client, ERR_NOSUCHCHANNEL, "Invalid channel name");
			throw std::runtime_error("[LOG] Name channel must start with #");
		}

		// Check if channel already exist
		std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());

		// If channel exist
		if (it != _channels.end()) {
			// Check channel is on invite only (+i)
			if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd())) {
				sendReply(*client, ERR_INVITEONLYCHAN, "Channel is on invite only mode (+i)");
				throw std::runtime_error("[LOG] Channel is in invite only.");
			}

			// Check if the channel get a user limit and if there is too many user for that limit (+l)
			if (it->second.getUserLimit() != -1 && it->second.getMemberCount() >= it->second.getUserLimit()) {
				sendReply(*client, ERR_CHANNELISFULL, "Channel is full, you cannot join it (+l)");
				throw std::runtime_error("[LOG] Channel is full.");
			}

			// Check if channel need a key to be joined (+k)
			if (it->second.getKey().size() > 0) {
				sendReply(*client, ERR_BADCHANNELKEY, "This channel need a key to join it");
				throw std::runtime_error("[LOG] This channel need a key");
			}

			// If every guard is OK, join the channel
			it->second.addMember(client);
			std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
			broadcast(it->second, client, message);
		}

		// If channel doesn't exist yet
		else {
			// Create new channel
			Channel channel(getChan->getName());

			// Init new channel
			channel.addMember(client); // Add the new member
			channel.addOperator(client); // By default the first user is a channel operator
			_channels.insert(std::make_pair(getChan->getName(), channel)); // Add channel to channel list

			// Send message
			std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
			broadcast(channel, client, message);
			continue;
		}
	}
}

void Server::join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client, Parser &parser) {
	size_t i = 0;

	// Parse every channels
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		// Check if user has joined too many channels or not
		if (client->getJoinedChannels().size() > 15) {
			sendReply(*client, ERR_TOOMANYCHANNELS, "You have joined too many channel");
			throw std::runtime_error("[LOG] User joined too many channels");
		}

		// Check if channel name is correct
		if (!parser.checkChannelName(getChan->getName())) {
			sendReply(*client, ERR_NOSUCHCHANNEL, "Invalid channel name");
			throw std::runtime_error("[LOG] Name channel must start with #");
		}

		// Check if there still is somes key in the given list
		if (i < listKey.size()) {
			// Check if channel alread exist
			std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());

			// If channel exist
			if (it != _channels.end()) {
				// Check if channel is on invite only (+i)
				if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd())) {
					sendReply(*client, ERR_INVITEONLYCHAN, "Channel is on invite only mode (+i)");
					throw std::runtime_error("[LOG] Channel is in invite only");
				}

				// Check if the channel get a user limit and if there is too many user for that limit (+l)
				if (it->second.getUserLimit() != -1 && it->second.getMemberCount() >= it->second.getUserLimit()) {
					sendReply(*client, ERR_CHANNELISFULL, "Channel is full, you cannot join it (+l)");
					throw std::runtime_error("[LOG] Channel is full");
				}

				// Check if the key is valid
				if (it->second.getKey() != listKey[i]) {
					sendReply(*client, ERR_BADCHANNELKEY, "Invalid channel key");
					throw std::runtime_error("User cannot join the channel : key error");
				}

				// If every guard are OK, user can join the channel
				it->second.addMember(client);
				std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
				broadcast(it->second, client, message);
			}

			// If channel doesn't exist yet
			else {
				// Create channel
				Channel channel(getChan->getName(), listKey[i]);

				// Init channel
				channel.addMember(client); // Add the first member
				channel.addOperator(client); // By default the first member is a channel operator
				_channels.insert(std::make_pair(getChan->getName(), channel)); // Insert channel in channel list

				// Send message
				std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
				broadcast(channel, client, message);
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
