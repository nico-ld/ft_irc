/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:11:39 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/29 15:19:58 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
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

// Forward declarations to avoid circular dependency
class User;
class Channel;

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
};

#endif
