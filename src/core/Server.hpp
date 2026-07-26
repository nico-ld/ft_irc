/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:11:39 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/26 15:26:02 by afons            ###   ########.fr       */
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
    int         _port;
    std::string _password;

    int         _serverFd;
    int         _epollFd;

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
    void     createChannel(const Channel &channel);
    void     removeChannel(const std::string& name);
    Channel* getChannelByName(const std::string& name);

    // --- Getters / Setters ---
    int                getPort() const;
    const std::string& getPassword() const;

    // COMMANDES 
    void join(std::vector<Channel> &listChannel, std::vector<std::string> &listKey, User *client);
    void join(std::vector<Channel> &listChannel, User *client);
    void kick(Channel &channel, User *kicked, const User *op);
    void kick(Channel &channel, User *kicked, std::string reason, const User *op);

    // Message
    void broadcastServer(std::string message);
    void broadcast(const Channel &channel, std::string message);
    void broadcast(const Channel &channel, const User *user, std::string message);
    void notification(const User *user, std::string message);
    void privateMessage(const User *src, const User *dest, std::string message);

};

#endif
