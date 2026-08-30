/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:11:39 by jdessoli          #+#    #+#             */
/*   Updated: 2026/08/27 09:56:05 by nico             ###   ########.fr       */
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
#define loop while(1)

class Parser;

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

		// FDs whose send() has hit a fatal error (EPIPE/ECONNRESET/...) and are
		// waiting to be cleaned up. Removal is deferred rather than done inline
		// from queueWrite()/flushWrite(), since those are called while iterating
		// _users/_channels members (broadcastServer/broadcast) and removeUser()
		// erases from _users - doing it mid-iteration would invalidate iterators.
		std::vector<int> _pendingRemovals;

		// Prevent copying
		Server(const Server &src) { (void)src; }
		Server &operator=(const Server &src) { (void)src; return (*this);}

		// === WRITE BACKPRESSURE (NetworkBuffer) ===
		/* > Toggle whether epoll also watches this fd for write-readiness (EPOLLOUT) */
		void setEpollWriteInterest(int fd, bool enable);

		/* > true if errno corresponds to a dead connection (vs. a transient
		     EAGAIN/EWOULDBLOCK/EINTR that's worth queuing and retrying) */
		bool isFatalSendError(int err) const;

		/* > Queue clientFd for removeUser() cleanup once it's safe to do so
		     (i.e. once we're not in the middle of iterating _users/_channels) */
		void scheduleRemoval(int fd, const std::string &reason);

		/* > Run removeUser() for every fd queued by scheduleRemoval(). Called
		     once per startLoop() pass, after all events have been handled. */
		void processPendingRemovals();

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
		void removeUser(int clientFd, std::string reason);


		// === WRITE BACKPRESSURE (NetworkBuffer) ===
		/* > Send data to a user without blocking: sends what it can immediately,
		     queues whatever the kernel wouldn't take yet, and arms EPOLLOUT so
		     the rest goes out once the socket is writable again. */
		void queueWrite(User &user, const std::string &data);

		/* > Called on an EPOLLOUT-ready event: flushes a user's pending outgoing buffer */
		void flushWrite(int fd);


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
		
		
		// === COMMANDS ===
		// JOIN
		void join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client, Parser &parser);
		void join(std::vector<Channel> &listChannel, User *client, Parser &parser);

		// KICK
		void kick(Channel &channel, User *kicked, const User *op, Parser &parser);
		void kick(Channel &channel, User *kicked, std::string reason, const User *op, Parser &parser);

		// TOPIC
		void topic(const Channel &channel, User *user);
		void topic(Channel &channel, std::string newTopic, User *user);

		// PART
		void part(std::vector<Channel> &channels, User *user);
		void part(std::vector<Channel> &channels, std::string reason, User *user);

		// INVITE
		void invite(const std::string &nickname, Channel &channel, const User *user, Parser &parser);

		// MODE
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
		void	sendReply(const User &user, const std::string &code, const std::string &rest);
};

#endif
