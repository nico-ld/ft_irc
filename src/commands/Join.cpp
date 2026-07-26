#include "../core/Server.hpp"
#include "../core/User.hpp"
#include "../core/Channel.hpp"
#include <stdexcept>

void Server::join(std::vector<Channel> &listChannel, User *client) {
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());
		if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd())) {
			throw std::runtime_error("ERR_INVITEONLYCHAN");
		}
		if (it->second.getUserLimit() != -1 && it->second.getMemberCount() > it->second.getUserLimit()) {
			throw std::runtime_error("ERR_CHANNELISFULL");
		}
		
		if (it != _channels.end()) {
			it->second.addMember(client);
			std::string message = " JOIN " + getChan->getName() + '\n';
			broadcast(it->second, client, message);
		}
		else {
			Channel channel(getChan->getName());
			channel.addMember(client);
			_channels.insert(std::make_pair(getChan->getName(), channel));
			std::string message = " JOIN " + getChan->getName() + '\n';
			broadcast(channel, client, message);
		}
	}
}

void Server::join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client) {

	size_t i = 0;
	for (std::vector<Channel>::iterator getChan = listChannel.begin(); getChan != listChannel.end(); ++getChan) {
		if (i < listKey.size()) {
			std::map<std::string, Channel>::iterator it = _channels.find(getChan->getName());
			if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd())) {
			throw std::runtime_error("ERR_INVITEONLYCHAN");
			}
			if (it->second.getUserLimit() != -1 && it->second.getMemberCount() > it->second.getUserLimit()) {
				throw std::runtime_error("ERR_CHANNELISFULL");
			}

			if (it != _channels.end()) {
				if (it->second.getKey() != listKey[i]) {
					notification(client, " cannot join the channel : key error\n");
					throw std::runtime_error("ERR_BADCHANNELKEY");
				}
				std::string message = " JOIN " + getChan->getName() + '\n';
				broadcast(it->second, client, message);
				it->second.addMember(client);
			}
			else {
				Channel channel(getChan->getName(), listKey[i]);
				channel.addMember(client);
				_channels.insert(std::make_pair(getChan->getName(), channel));
				std::string message = " JOIN " + getChan->getName() + '\n';
				broadcast(channel, client, message);
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
