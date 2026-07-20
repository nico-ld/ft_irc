/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 21:20:20 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/20 16:46:10 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include <algorithm> // For std::find

// ==========================================
// 1. CONSTRUCTORS & DESTRUCTOR
// ==========================================

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

// Orthodox Canonical Form: Copy Constructor
User::User(const User& src) {
    *this = src;
}

// Orthodox Canonical Form: Copy Assignment Operator
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
    }
    return *this;
}

// ==========================================
// 2. GETTERS & SETTERS (Person A, B, & C)
// ==========================================

int User::getFd() const { return _fd; }

const std::string& User::getHostname() const { return _hostname; }
void User::setHostname(const std::string& hostname) { _hostname = hostname; }

const std::string& User::getNickname() const { return _nickname; }
void User::setNickname(const std::string& nickname) { _nickname = nickname; }

const std::string& User::getUsername() const { return _username; }
void User::setUsername(const std::string& username) { _username = username; }

const std::string& User::getRealname() const { return _realname; }
void User::setRealname(const std::string& realname) { _realname = realname; }

// ==========================================
// 3. STATE HANDLERS (Person B)
// ==========================================

bool User::hasProvidedPassword() const { return _hasProvidedPassword; }
void User::setProvidedPassword(bool state) { _hasProvidedPassword = state; }

bool User::hasProvidedNick() const { return _hasProvidedNick; }
void User::setProvidedNick(bool state) { _hasProvidedNick = state; }

bool User::hasProvidedUser() const { return _hasProvidedUser; }
void User::setProvidedUser(bool state) { _hasProvidedUser = state; }

bool User::isAuthenticated() const { return _isAuthenticated; }
void User::setAuthenticated(bool state) { _isAuthenticated = state; }

// ==========================================
// 4. CHANNEL HELPERS (Person C)
// ==========================================

const std::vector<std::string>& User::getJoinedChannels() const { 
    return _joinedChannels; 
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
