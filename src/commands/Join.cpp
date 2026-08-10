#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::join(std::vector<Channel> &listChannel, User *client) {
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		if (!Parser::checkNameChannel(getChan->getName()))
			throw std::runtime_error("Name channel must start with #");

		std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());
		if (it != _channels.end()) {
			if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd()))
				throw std::runtime_error("ERR_INVITEONLYCHAN");

			if (it->second.getUserLimit() != -1 && it->second.getMemberCount() >= it->second.getUserLimit())
				throw std::runtime_error("ERR_CHANNELISFULL");

			if (it->second.getKey().size() > 0)
				throw std::runtime_error("This channel need a key");

			it->second.addMember(client);
			std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
			broadcast(it->second, client, message);
		}
		else {
			Channel channel(getChan->getName());
			channel.addMember(client);
			_channels.insert(std::make_pair(getChan->getName(), channel));
			std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
			broadcast(channel, client, message);
			continue;
		}
	}
}

void Server::join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client) {
	size_t i = 0;
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		if (!Parser::checkNameChannel(getChan->getName()))
			throw std::runtime_error("Name channel must start with #");

		if (i < listKey.size()) {
			std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());
			if (it != _channels.end()) {

				if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd()))
					throw std::runtime_error("ERR_INVITEONLYCHAN");
				if (it->second.getUserLimit() != -1 && it->second.getMemberCount() > it->second.getUserLimit())
					throw std::runtime_error("ERR_CHANNELISFULL");
				if (it->second.getKey() != listKey[i]) {
					notification(client, " cannot join the channel : key error\n");
					throw std::runtime_error("ERR_BADCHANNELKEY");
				}

				std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
				broadcast(it->second, client, message);
				it->second.addMember(client);
				}

			else {
				Channel channel(getChan->getName(), listKey[i]);
				channel.addMember(client);
				_channels.insert(std::make_pair(getChan->getName(), channel));
				std::string message = client->getNickname() + " joined " + getChan->getName() + '\n';
				broadcast(channel, client, message);
				continue;
			}	
		}
		else {
			std::vector<Channel> restOfListChannel(getChan, listChannel.end()); 
			join(restOfListChannel, client);
			return ;
		}
		++i;
	}
}
