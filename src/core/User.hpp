/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 21:19:09 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/15 21:19:21 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
private:
    // 1. Hardware Connection Details (Person A)
    int         _fd;
    std::string _hostname; // Client's IP or hostname (useful for welcome messages)

    // 2. Identity Information (Person B)
    std::string _nickname;
    std::string _username;
    std::string _realname;

    // 3. State Flags for Authentication Handshake (Person B)
    bool _hasProvidedPassword;
    bool _hasProvidedNick;
    bool _hasProvidedUser;
    bool _isAuthenticated; // Becomes true only when the top three are true

    // 4. Channel Tracking (Person C)
    // Stores names of channels this user is currently in
    std::vector<std::string> _joinedChannels; 

public:
    // Constructor / Destructor
    explicit User(int fd); // Explicit prevents accidental conversions
    ~User();

    // Copy Constructor & Assignment Operator (Orthodox Canonical Form)
    User(const User& src);
    User& operator=(const User& src);

    // --- Getters & Setters ---
    int                getFd() const;
    const std::string& getHostname() const;
    void               setHostname(const std::string& hostname);

    const std::string& getNickname() const;
    void               setNickname(const std::string& nickname);

    const std::string& getUsername() const;
    void               setUsername(const std::string& username);

    const std::string& getRealname() const;
    void               setRealname(const std::string& realname);

    // --- State Handlers (Crucial for Person B's Authentication Logic) ---
    bool hasProvidedPassword() const;
    void setProvidedPassword(bool state);

    bool hasProvidedNick() const;
    void setProvidedNick(bool state);

    bool hasProvidedUser() const;
    void setProvidedUser(bool state);

    bool isAuthenticated() const;
    void setAuthenticated(bool state);

    // --- Channel Helpers (Crucial for Person C's Logic) ---
    const std::vector<std::string>& getJoinedChannels() const;
    void joinChannel(const std::string& channelName);
    void leaveChannel(const std::string& channelName);
};

#endif
