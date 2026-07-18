#include "Server.hpp"
#include <sys/socket.h>

void Server::join(std::string channel, const User &client) {
    std::map<std::string, Channel>::iterator it = _channels.find(channel);
    if (it != _channels.end())
        it->second.addClient(client);
    else {
        Channel channel(nameChannel);
        channel.addClient(client);
        _channels.insert(std::make_pair(nameChannel, channel));
    }
}

void Server::join(std::string nameChannel, std::string key, const User &client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);
    if (it != _channels.end()) {

        if (it->second.getKey() != key) {
            std::string messageError = client + " cannot join the channel : key error\n";
            // if (send(client.fd, messageError.c_str(), messageError.size(), 0) == -1)

            return ;
        }

        it->second.addClient(client);
    }
    else {
        Channel channel(nameChannel, key);
        channel.addClient(client);
        _channels.insert(std::make_pair(nameChannel, channel));
    }
}
