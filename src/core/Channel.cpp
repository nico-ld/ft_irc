/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:50:45 by jdessoli          #+#    #+#             */
/*   Updated: 2026/09/04 10:38:59 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"

// ==========================================
// 1. CONSTRUCTORS & DESTRUCTOR
// ==========================================

Channel::Channel(const std::string& name)
    : _name(name),
      _topic(""),
      _isInviteOnly(false),
      _isTopicRestricted(true), // Standard IRC default: usually only ops change topic (+t)
      _key(""),
      _userLimit(-1) {} // -1 means no limit is enforced

Channel::Channel(const std::string& name, const std::string& key)
    : _name(name),
      _topic(""),
      _isInviteOnly(false),
      _isTopicRestricted(true), // Standard IRC default: usually only ops change topic (+t)
      _key(key),
      _userLimit(-1) {}
Channel::~Channel() {}

// Orthodox Canonical Form: Copy Constructor
Channel::Channel(const Channel& src) {
    *this = src;
}


// Orthodox Canonical Form: Copy Assignment Operator
Channel& Channel::operator=(const Channel& src) {
    if (this != &src) {
        _name = src._name;
        _topic = src._topic;
        _members = src._members;
        _operators = src._operators;
        _invitedUsers = src._invitedUsers;
        _isInviteOnly = src._isInviteOnly;
        _isTopicRestricted = src._isTopicRestricted;
        _key = src._key;
        _userLimit = src._userLimit;
    }
    return (*this);
}

// ==========================================
// 2. CORE GETTERS & SETTERS
// ==========================================

const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }
void Channel::setTopic(const std::string& topic) { _topic = topic; }

// ==========================================
// 3. MEMBERSHIP MANAGEMENT (Person C)
// ==========================================

void Channel::addMember(User* user) {
    if (user) {
        _members[user->getFd()] = user;
    }
}

void Channel::removeMember(User* user) {
    if (user) {
        int fd = user->getFd();
        _members.erase(fd);
        _operators.erase(fd);
        _invitedUsers.erase(fd);
        user->leaveChannel(_name);
    }
}

bool Channel::isMember(int fd) const {
    return (_members.find(fd) != _members.end());
}

const std::map<int, User*>& Channel::getMembers() const {
    return (_members);
}

int Channel::getMemberCount() const {
    return (_members.size());
}

// ==========================================
// 4. OPERATOR MANAGEMENT (Person C)
// ==========================================

void Channel::addOperator(User* user) {
    if (user && isMember(user->getFd())) {
        _operators[user->getFd()] = user;
    }
}

void Channel::removeOperator(User* user) {
    if (user) {
        _operators.erase(user->getFd());
    }
}

bool Channel::isOperator(int fd) const {
    return (_operators.find(fd) != _operators.end());
}

// ==========================================
// 5. INVITATION MANAGEMENT (Person C)
// ==========================================

void Channel::inviteUser(User* user) {
    if (user) {
        _invitedUsers[user->getFd()] = user;
    }
}

bool Channel::isInvited(int fd) const {
    return (_invitedUsers.find(fd) != _invitedUsers.end());
}

void Channel::clearInvite(int fd) {
    _invitedUsers.erase(fd);
}

// ==========================================
// 6. CHANNEL MODE CONFIGURATIONS (Person C)
// ==========================================

bool Channel::isInviteOnly() const { return (_isInviteOnly); }
void Channel::setInviteOnly(bool state) { _isInviteOnly = state; }

bool Channel::isTopicRestricted() const { return (_isTopicRestricted); }
void Channel::setTopicRestricted(bool state) { _isTopicRestricted = state; }

const std::string& Channel::getKey() const { return (_key); }
void Channel::setKey(const std::string& key) { _key = key; }

int Channel::getUserLimit() const { return (_userLimit); }
void Channel::setUserLimit(long limit) { _userLimit = limit; }
