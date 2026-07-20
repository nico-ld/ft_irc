#include "Server.hpp"
#include "User.hpp"
#include "../core/Channel.hpp"
#include <sys/socket.h>

void Server::join(std::string nameChannel, User *client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);
    if (it != _channels.end())
        it->second.addMember(client);
    else {
        Channel channel(nameChannel);
        channel.addMember(client);
        _channels.insert(std::make_pair(nameChannel, channel));
    }
}
    
void Server::join(std::string nameChannel, std::string key, User *client) {
    std::map<std::string, Channel>::iterator it = _channels.find(nameChannel);
    if (it != _channels.end()) {
        if (it->second.getKey() != key) {
            std::string messageError = client->getUsername() + " cannot join the channel : key error\n";
            if (send(client->getFd(), messageError.c_str(), messageError.size(), 0) == -1)
                perror("Send crashed.");
            return ;
        }
        it->second.addMember(client);
    }
    else {
        Channel channel(nameChannel, key);
        channel.addMember(client);
        _channels.insert(std::make_pair(nameChannel, channel));
    }
}
