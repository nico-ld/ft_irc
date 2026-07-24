/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:13:26 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/24 09:46:12 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"


#define MAX_EVENTS 64
#define BUFFER_SIZE 512
#define loop while(1)

//serverFd and epollFd are set at -1, because that's a Unix convention saying the fd is closed / not init
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

	// 	The bind function is used to claim a port, in order to receive traffic
    if (bind(_serverFd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Bind error: unable to bind the server to a port");

	// Listen takes to args, the socket that receive packets, and the size of the queue 
    // SOMAXCONN means we give maximum size allowed by the OS for the queue
    if (listen(_serverFd, SOMAXCONN) < 0)
        throw std::runtime_error("Listen error : unable to set the server to listen");

    // Create epoll instance, and instance is a monitoring tool that warns if there's
	// a change in the monitored socket (more efficient memory-wise that multi-threading)
	// The parameter is a flag, 0 means standart behavior for an epoll instance
    _epollFd = epoll_create1(0);
    if (_epollFd < 0) throw std::runtime_error("Epoll_create1 error : failed to create epoll instance");

    // Add master server socket to epoll
	// The struct tells the kernel what to watch for (EPOLLIN = read data) 
	// and which file descriptor is associated with that event (data.fd = _serverFd)
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = _serverFd;

	// epoll_ctl is the API to control the dashboard built by epoll_create1
	// 4 args: dashboard, operation to do, target of the operation, pointer to the epoll_event struct
	// Here, we're _serverFd to the watchlist
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &ev) < 0)
        throw std::runtime_error("epoll_ctl error : failed to add master socket to watchlist");

    std::cout << "IRC Server successfully launched on port " << _port << std::endl;
}

void Server::startLoop() {
    struct epoll_event events[MAX_EVENTS];

    loop {
        // Pause the socket until an event happens
		// _epollFd = the socket to watch over, events = An array of struct epoll_event
		// MAX_EVENTS = the size of the events array
		// -1 = it's the timeout arg, -1 means the socket is under sleep indefinitely until an event happen
        int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
        if (numEvents < 0) {
            std::cerr << "epoll_wait error: unable to pause the socket" << std::endl;
            break; 
        }

		// Pulls out the currentFd that triggered an event
        for (int i = 0; i < numEvents; ++i) {
            int currentFd = events[i].data.fd;

            // Case A: currentFd == _serverFd, meaning currentFd is a new client
			// The OS picks up the new connection, create a socket for the client
			// then adds it to the epoll list and register it as Unauthenticated User for now
            if (currentFd == _serverFd) {
                struct sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(clientAddr);
                int clientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &addrLen);
                if (clientFd >= 0) {
                    fcntl(clientFd, F_SETFL, O_NONBLOCK);

                    struct epoll_event ev;
                    memset(&ev, 0, sizeof(ev));
                    ev.events = EPOLLIN;
                    ev.data.fd = clientFd;
                    epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &ev);

                    addUnauthenticatedUser(clientFd);
                    std::cout << "New client connected on fd: " << clientFd << std::endl;
                }
            } 
            // Case B: the incoming client is already connected and registered through epoll
			// events[i].events & EPOLLIN means there are incoming bytes waiting from currentFd
			// We therefore create a buffer in which we'll receive all the incoming bytes
            else if (events[i].events & EPOLLIN) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);
                
                int bytesRead = recv(currentFd, buffer, BUFFER_SIZE - 1, 0);

				// if <= 0, it means the network disconnected during the process
                if (bytesRead <= 0) {
                    std::cout << "Client disconnected on fd: " << currentFd << std::endl;
                    removeUser(currentFd);
                } else {
					std::map<int, User>::iterator currentUser = _users.find(currentFd);
					if (currentUser == _users.end())
						throw std::runtime_error("Error: User not found");
					
    				// Append the newly read bytes to the client's buffer
					// Then process all complete commands (so closed with either \r\n or \n) in the buffer
					currentUser->second.inputBuffer.append(buffer, bytesRead);
    				size_t pos;
    				while ((pos = currentUser->second.inputBuffer.find("\r\n")) != std::string::npos) {
        				// Extract the complete command string (excluding the \r\n)
						// Then erase the extracted command and the \r\n delim from the client's buffer
						// To finally execute the command, such as IRC PASS, NICK or JOIN
        				std::string command = currentUser->second.inputBuffer.substr(0, pos);
        				currentUser->second.inputBuffer.erase(0, pos + 2);
        				if (!command.empty()) {
							if (Parser::parse(command) == 0)
								dispatchCommand(currentUser->second);
						}
            		}
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

void Server::addUnauthenticatedUser(int clientFd) {
    _users.insert(std::make_pair(clientFd, User(clientFd)));
}

void Server::removeUser(int clientFd) {
    epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL);
    close(clientFd);
    _users.erase(clientFd);
}

User* Server::getUserById(int fd) {
    std::map<int, User>::iterator it = _users.find(fd);
    if (it != _users.end()) {
        return &(it->second);
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
