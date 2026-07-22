#include "../core/Server.hpp"
#include "../core/User.hpp"
#include "../core/Channel.hpp"
#include <stdexcept>

void Server::join(std::string nameChannel, User *client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);
    if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd())) {
        throw std::runtime_error("ERR_INVITEONLYCHAN");
    }
    if (it->second.getUserLimit() != -1 && it->second.getMemberCount() > it->second.getUserLimit()) {
        throw std::runtime_error("ERR_CHANNELISFULL");
    }

    if (it != _channels.end()) {
        it->second.addMember(client);
        std::string message = " JOIN " + nameChannel + '\n';
        broadcast(it->second, client, message);
    }
    else {
        Channel channel(nameChannel);
        channel.addMember(client);
        _channels.insert(std::make_pair(nameChannel, channel));
        std::string message = " JOIN " + nameChannel + '\n';
        broadcast(channel, client, message);
    }
}

void Server::join(std::string nameChannel, std::string key, User *client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);

    if (it->second.isInviteOnly() && !it->second.isInvited(client->getFd())) {
        throw std::runtime_error("ERR_INVITEONLYCHAN");
    }
    if (it->second.getUserLimit() != -1 && it->second.getMemberCount() > it->second.getUserLimit()) {
        throw std::runtime_error("ERR_CHANNELISFULL");
    }

    if (it != _channels.end()) {
        if (it->second.getKey() != key) {
            notification(client, " cannot join the channel : key error\n");
            throw std::runtime_error("ERR_BADCHANNELKEY");
        }
        std::string message = " JOIN " + nameChannel + '\n';
        broadcast(it->second, client, message);
        it->second.addMember(client);
    }
    else {
        Channel channel(nameChannel, key);
        channel.addMember(client);
        _channels.insert(std::make_pair(nameChannel, channel));
        std::string message = " JOIN " + nameChannel + '\n';
        broadcast(channel, client, message);
    }
}
