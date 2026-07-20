#include "core/Server.hpp"
#include "core/User.hpp"
#include "core/Channel.hpp"

void Server::join(std::string nameChannel, User *client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);
    if (it != _channels.end())
        it->second.addMember(client);
    else {
        Channel channel(nameChannel);
        channel.addMember(client);
        _channels.insert(std::make_pair(nameChannel, channel));
    }
    std::string message = " JOIN " + nameChannel + '\n';
    broadcast(client, message);
}

void Server::join(std::string nameChannel, std::string key, User *client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);
    if (it != _channels.end()) {
        if (it->second.getKey() != key) {
            broadcast(client, " cannot join the channel : key error\n");
            return ;
        }
        it->second.addMember(client);
    }
    else {
        Channel channel(nameChannel, key);
        channel.addMember(client);
        _channels.insert(std::make_pair(nameChannel, channel));
    }
    std::string message = " JOIN " + nameChannel + '\n';
    broadcast(client, message);
}
