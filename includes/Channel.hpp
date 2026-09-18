/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:49:37 by jdessoli          #+#    #+#             */
/*   Updated: 2026/09/18 07:33:42 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>

class User; // Forward declaration needed as Channel needs User

class Channel {
private:
    std::string _name;
    std::string _topic;

    // Relationships: We map User socket FDs to their actual object pointers
    std::map<int, User*> _members;
    std::map<int, User*> _operators;
    std::map<int, User*> _invitedUsers; // Useful for the invite-only (+i) mode

    // --- Channel Modes Flags (Crucial for Person C) ---
    bool        _isInviteOnly;       // +i
    bool        _isTopicRestricted;  // +t (true = only ops can change topic)
    std::string _key;                // +k (empty string means no password)
    long        _userLimit;          // +l (-1 means no limit active)

public:
    Channel(const std::string& name);
    Channel(const std::string& name, const std::string& key);
    ~Channel();
    Channel(const Channel& src);
    Channel& operator=(const Channel& src);

    const std::string& getName() const;
    const std::string& getTopic() const;
    void               setTopic(const std::string& topic);

    void addMember(User* user);
    void removeMember(User* user);
    bool isMember(int fd) const;
    const std::map<int, User*>& getMembers() const;

    void addOperator(User* user);
    void removeOperator(User* user);
    bool isOperator(int fd) const;

    void inviteUser(User* user);
    bool isInvited(int fd) const;
    void clearInvite(int fd);

    bool               isInviteOnly() const;
    void               setInviteOnly(bool state);

    bool               isTopicRestricted() const;
    void               setTopicRestricted(bool state);

    const std::string& getKey() const;
    void               setKey(const std::string& key);

    int               getUserLimit() const;
    void              setUserLimit(long limit);
    int               getMemberCount() const;
};

#endif
