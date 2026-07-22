/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 21:19:09 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/22 08:04:52 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
	public:
		// == Constructors & Destructor ==
		User(int fd);
		User(const User& src);
		~User();
		
		// == Overload ==
		User& operator=(const User& src);

		// === GETTERS & SETTERS ===
		/* > Return user's fd */
		int                getFd() const { return (_fd); }

		/* > Return user's hostname */
		const std::string& getHostname() const { return (_hostname); }
		/* > Set user's hostname */
		void               setHostname(const std::string& hostname) { _hostname = hostname; }

		/* > Return user's nickname */
		const std::string& getNickname() const { return (_nickname); }
		/* > Set user's nickname */
		void               setNickname(const std::string& nickname) { _nickname = nickname; }

		/* > Return user's name*/
		const std::string& getUsername() const { return (_username); }
		/* > Set user's name */
		void               setUsername(const std::string& username) { _username = username; }

		/* > Return user's realname */
		const std::string& getRealname() const { return (_realname); }
		/* > Set user's realname*/
		void               setRealname(const std::string& realname) { _realname = realname; }

		/* > Return user's prefix */
		const std::string& getPrefix() const { return (_userPrefix); }
		/* > Set user's prefix */
		void			   setPrefix(const std::string& prefix) { _userPrefix = prefix; }

		// === STATE HANDLERS ===
		bool hasProvidedPassword() const { return (_hasProvidedPassword); }
		void setProvidedPassword(bool state) { _hasProvidedPassword = state; }

		bool hasProvidedNick() const { return (_hasProvidedNick); }
		void setProvidedNick(bool state) { _hasProvidedNick = state; }

		bool hasProvidedUser() const { return (_hasProvidedUser); }
		void setProvidedUser(bool state) { _hasProvidedUser = state; }

		bool isAuthenticated() const { return (_isAuthenticated); }
		void setAuthenticated(bool state) { _isAuthenticated = state; }

		// === CHANNEL HELPERS ===
		/* > Return the list of channels joined by the user */
		const std::vector<std::string>& getJoinedChannels() const { return (_joinedChannels); }
		
		void joinChannel(const std::string& channelName);
		void leaveChannel(const std::string& channelName);

	private:
		// Hardware Connection Details
		int         _fd;
		std::string _hostname;

		// Identity Information
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _userPrefix;

		//State Flags for Authentication Handshake
		bool _hasProvidedPassword;
		bool _hasProvidedNick;
		bool _hasProvidedUser;
		bool _isAuthenticated; // Becomes true only when the top three are true

		// Channel Tracking
		std::vector<std::string> _joinedChannels; // Stores names of channels this user is currently in
};

#endif
