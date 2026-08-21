/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:11:39 by jdessoli          #+#    #+#             */
/*   Updated: 2026/08/19 12:11:03 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Replies.hpp"
#include "Channel.hpp"
#include "User.hpp"

#include <map>
#include <vector>
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>

#define MAX_EVENTS 64
#define BUFFER_SIZE 512
#define MAX_USER 20
#define loop while(1)

// Forward declarations to avoid circular dependency
// class User;
// class Channel;

class Server
{
	private:
		int _port;
		std::string _password;

		int _serverFd;
		int _epollFd;

		// Central Memory Storage (The "Database")
		std::map<int, User> _users;				  // Key: client socket FD -> Value: User object
		std::map<std::string, Channel> _channels; // Key: Channel Name -> Value: Channel object

		// Prevent copying
		Server(const Server &src) { (void)src; }
		Server &operator=(const Server &src) { (void)src; return (*this);}

	public:
		// == Constructor & Destructor == 
		Server(int port, const std::string &password);
		~Server();

		
		// === 	CORE SERVER LIFECYCLE ===
		/* > Set up sockets & epoll */
		void init();
		
		/* > The infinite epoll_wait loop */
		void startLoop();

		/* > Clean up all FDs and memory */
		void stop();


		// === USER MANAGEMENT ===
		/* > Inserts a blank User object mapping to the socket FD */
		void addUnauthenticatedUser(int clientFd);
		
		/* > Remove client FD from epoll tracking & delete user */
		void removeUser(int clientFd);


		// === CHANNEL MANAGEMENT ===
		/* > Create a new Channel and add it to channel list */
		void createChannel(const std::string &name, User *creator);

		/* > Delete the channel */
		void removeChannel(const std::string &name);
		
		// === GETTERS & SETTERS ===
		/* > Return the server port */
		int getPort() const { return (_port); }
		
		/* > Return the server password */
		const std::string &getPassword() const { return (_password); }
		
		/* > Return an User object */
		User *getUserById(int fd);
		
		/* > Return an User object */
		User *getUserByNickname(const std::string& nickname);
		
		/* > Return a Channel object */
		Channel *getChannelByName(const std::string &name);
		
		// COMMANDES 
		void join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client);
		void join(std::vector<Channel> &listChannel, User *client);
		void kick(Channel &channel, User *kicked, const User *op);
		void kick(Channel &channel, User *kicked, std::string reason, const User *op);
		void topic(const Channel &channel, User *user);
		void topic(Channel &channel, std::string newTopic, User *user);
		void part(std::vector<Channel> &channels, User *user);
		void part(std::vector<Channel> &channels, std::string reason, User *user);
		void invite(const std::string &nickname, Channel &channel, const User *user);
		void mode(Channel &channel, std::string listMode, User *user, std::vector<std::string> params = std::vector<std::string>());
		void launchMode(Channel &channel, std::vector<std::string> modestring, std::vector<std::string> params, User *user);

		// Message
		void broadcastServer(std::string message);
		void broadcast(const Channel &channel, std::string message);
		void broadcast(const Channel &channel, const User *user, std::string message);
		void notification(const User *user, std::string message);
		void privateMessageUser(const User *src, const User *dest, std::string message);
		void privateMessageChannel(const User *src, const Channel &channel, std::string message);


		// === NULERIC REPLIES ===
		/* > Send to User a numeric reply of last command */
		void	sendReply(User &user, const std::string &code, const std::string &rest);
};

#endif
