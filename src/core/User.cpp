/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 21:20:20 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/22 07:53:47 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include <algorithm>

User::User(int fd) 
    : _fd(fd), 
      _hostname("127.0.0.1"), // Default to local, NetworkManager can overwrite this
      _nickname(""), 
      _username(""), 
      _realname(""),
      _hasProvidedPassword(false), 
      _hasProvidedNick(false), 
      _hasProvidedUser(false), 
      _isAuthenticated(false) {}

User::~User() {}

User::User(const User& src) {
    *this = src;
}

User& User::operator=(const User& src) {
    if (this != &src) {
        _fd = src._fd;
        _hostname = src._hostname;
        _nickname = src._nickname;
        _username = src._username;
        _realname = src._realname;
        _hasProvidedPassword = src._hasProvidedPassword;
        _hasProvidedNick = src._hasProvidedNick;
        _hasProvidedUser = src._hasProvidedUser;
        _isAuthenticated = src._isAuthenticated;
        _joinedChannels = src._joinedChannels;
        _netBuffer = src._netBuffer;
    }
    return *this;
}

void User::joinChannel(const std::string& channelName) {
    // Check if the channel is already in the list to avoid duplicate entries
    if (std::find(_joinedChannels.begin(), _joinedChannels.end(), channelName) == _joinedChannels.end()) {
        _joinedChannels.push_back(channelName);
    }
}

void User::leaveChannel(const std::string& channelName) {
    std::vector<std::string>::iterator it = std::find(_joinedChannels.begin(), _joinedChannels.end(), channelName);
    if (it != _joinedChannels.end()) {
        _joinedChannels.erase(it);
    }
}
