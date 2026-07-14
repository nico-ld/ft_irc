/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:11:39 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/14 21:12:17 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <sys/epoll.h>

// Forward declarations to avoid circular dependency
class User;
class Channel;

class Server {
private:
    // 1. Immutable Server Configuration
    int         _port;
    std::string _password;

    // 2. Network / Socket Descriptors (Person A's Territory)
    int         _serverFd; // The master listening socket
    int         _epollFd;  // The epoll instance file descriptor

    // 3. Central Memory Storage (The "Database")
    std::map<int, User>            _users;    // Key: client socket FD -> Value: User object
    std::map<std::string, Channel> _channels; // Key: Channel Name -> Value: Channel object

    // Prevent copying (Rule of Three in C++98)
    Server(const Server& src);
    Server& operator=(const Server& src);

public:
    // Constructor / Destructor
    Server(int port, const std::string& password);
    ~Server();

    // --- Core Server Lifecycle (Person A) ---
    void initServer(); // Set up sockets & epoll
    void startLoop();  // The infinite epoll_wait loop
    void stopServer(); // Clean up all FDs and memory

    // --- User Management (For Person A, B, & C) ---
    void  addUnauthenticatedUser(int clientFd);
    void  removeUser(int clientFd); // Handles socket close & cleanup from _users
    
    // Lookups (Crucial for Person B and C)
    User* getUserById(int fd);
    User* getUserByNickname(const std::string& nickname);

    // --- Channel Management (For Person C) ---
    void     createChannel(const std::string& name, User* creator);
    void     removeChannel(const std::string& name);
    Channel* getChannelByName(const std::string& name);

    // --- Getters / Setters ---
    int                getPort() const;
    const std::string& getPassword() const;
};

#endif
