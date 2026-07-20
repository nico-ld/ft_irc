/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:13:26 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/20 13:08:45 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"      // Assuming Person A/B writes this
#include "Channel.hpp"   // Assuming Person A/C writes this
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_EVENTS 64
#define BUFFER_SIZE 512

Server::Server(int port, const std::string& password) 
    : _port(port), _password(password), _serverFd(-1), _epollFd(-1) {}

Server::~Server() {
    stopServer();
}

void Server::initServer() {
    // Turn _serverFd into a the master socket, meaning the first socket created (usually to listen)
	// AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol for those params
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0) throw std::runtime_error("socket error: failed to create socket");

    // Needed to restart the server without going through the TIME_WAIT syscall
	// _serverFd = configured socket, SOL_SOCKET = where to seek the options of the socket, 
	// SO_REUSEADDR = the rule we're giving to skip TIME_WAIT, &opt = pointer to the opt bool (system expect an int, not bool)
	// the sizeof operation is needed because setsockopt is generic, so we must tell it what we're sending 
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsockopt error: SO_REUSEADDR failed on master socket");
	}

    // Set master socket to non-blocking
	// _serverFd = fd to change permissions of, F_GETFL = gets the flags to be saved, so they're not overriden at the later if
	int flags = fcntl(_serverFd, F_GETFL, 0);
	if (flags < 0) {
    	throw std::runtime_error("fcntl error: F_GETFL failed on master socket");
	}

	// F_SETFL = tells to change / override the fd flags with the next args
	// O_NONBLOCK = configure to open fd in non blocking, so cmd like accept or send won't block program running
	if (fcntl(_serverFd, F_SETFL, flags | O_NONBLOCK) < 0) {
    	throw std::runtime_error("fcntl error: O_NONBLOCK failed on master socket");
	}

    // Bind socket to Port
	// sockaddr_in type comes from <netinet/in.h>
	// INADDR_ANY = set IP placeholder to 0.0.0.0, meaning it accepts connexions from any source, so wifi, ethernet and so on
	// The htons function converts the port number in Big-Endian, so it match what's expected on the Network protocol standart
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    if (bind(_serverFd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Bind failed");

    // Start listening
    if (listen(_serverFd, SOMAXCONN) < 0)
        throw std::runtime_error("Listen failed");

    // Create epoll instance
    _epollFd = epoll_create1(0);
    if (_epollFd < 0) throw std::runtime_error("Failed to create epoll instance");

    // Add master server socket to epoll
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN; // Watch for incoming connections
    ev.data.fd = _serverFd;
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &ev) < 0)
        throw std::runtime_error("epoll_ctl failed for master socket");

    std::cout << "IRC Server successfully launched on port " << _port << std::endl;
}

void Server::startLoop() {
    struct epoll_event events[MAX_EVENTS];

    while (true) {
        // Wait for OS network events (Blocks here until something happens)
        int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
        if (numEvents < 0) {
            std::cerr << "epoll_wait error" << std::endl;
            break; 
        }

        for (int i = 0; i < numEvents; ++i) {
            int currentFd = events[i].data.fd;

            // Scenario A: New Connection coming in on the listening socket
            if (currentFd == _serverFd) {
                struct sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(clientAddr);
                int clientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &addrLen);
                if (clientFd >= 0) {
                    // Set new client socket to non-blocking
                    fcntl(clientFd, F_SETFL, O_NONBLOCK);

                    // Add new client to epoll list
                    struct epoll_event ev;
                    memset(&ev, 0, sizeof(ev));
                    ev.events = EPOLLIN; // Watch for readable data from client
                    ev.data.fd = clientFd;
                    epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &ev);

                    // Instantiate a User in core memory
                    addUnauthenticatedUser(clientFd);
                    std::cout << "New client connected on fd: " << clientFd << std::endl;
                }
            } 
            // Scenario B: Read event on an existing client's socket
            else if (events[i].events & EPOLLIN) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);
                
                // Read exactly once (No loops checking errno, as per the subject!)
                int bytesRead = recv(currentFd, buffer, BUFFER_SIZE - 1, 0);

                if (bytesRead <= 0) {
                    // Client disconnected or read error
                    std::cout << "Client disconnected on fd: " << currentFd << std::endl;
                    removeUser(currentFd);
                } else {
                    // -> Hand off buffer string to BufferManager here! <-
                    // (e.g., BufferManager::append(currentFd, std::string(buffer, bytesRead)))
                }
            }
        }
    }
}

void Server::stopServer() {
    // Clean up connections
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it) {
        close(it->first);
    }
    _users.clear();
    _channels.clear();

    if (_serverFd != -1) close(_serverFd);
    if (_epollFd != -1) close(_epollFd);
}

// ==========================================
// 2. USER/DATABASE MANAGEMENT (Person A, B, & C)
// ==========================================

void Server::addUnauthenticatedUser(int clientFd) {
    // Inserts a blank User object mapping to the socket FD
    _users.insert(std::make_pair(clientFd, User(clientFd)));
}

void Server::removeUser(int clientFd) {
    // 1. Remove client FD from epoll tracking
    epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL);
    
    // 2. Close socket
    close(clientFd);

    // 3. Remove from local memory map
    _users.erase(clientFd);

    // Note: Person C will also need to clean up this user from any Channels!
}

User* Server::getUserById(int fd) {
    std::map<int, User>::iterator it = _users.find(fd);
    if (it != _users.end()) {
        return &(it->second); // Return memory address of existing object
    }
    return NULL;
}

User* Server::getUserByNickname(const std::string& nickname) {
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            return &(it->second);
        }
    }
    return NULL;
}

// ==========================================
// 3. GETTERS
// ==========================================
int Server::getPort() const { return _port; }
const std::string& Server::getPassword() const { return _password; }
