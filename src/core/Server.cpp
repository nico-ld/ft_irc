/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 21:13:26 by jdessoli          #+#    #+#             */
/*   Updated: 2026/09/01 15:41:31 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Channel.hpp"
#include "NetworkUtils.hpp"

volatile sig_atomic_t g_stop = 0;

void handleInterupt(int signal) {
	g_stop = signal;
}

//serverFd and epollFd are set at -1, because that's a Unix convention saying the fd is closed / not init
Server::Server(int port, const std::string& password) 
    : _port(port), _password(password), _serverFd(-1), _epollFd(-1) 
{
    dash = new Dashboard(IRCSERV, "ircserv.log");
    dash->render();

   	signal(SIGINT, handleInterupt);
}

Server::~Server() {
    stop();
    dash->log(SYSTEM, "Server is down");
    dash->updateInfo(dash->getSectionByIndex(0), LEFT, 0, "DOWN");
    delete dash;
}

void Server::init() {
    // Turn _serverFd into a the master socket, meaning the first socket created (usually to listen)
	// AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol for those params
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0) {
        dash->log(ERROR_LVL, "socket() error: failed to create socket");
        throw std::runtime_error("socket error: failed to create socket");
    }

    // Needed to restart the server without going through the TIME_WAIT syscall
	// _serverFd = configured socket, SOL_SOCKET = where to seek the options of the socket, 
	// SO_REUSEADDR = the rule we're giving to skip TIME_WAIT, &opt = pointer to the opt bool (system expect an int, not bool)
	// the sizeof operation is needed because setsockopt is generic, so we must tell it what we're sending 
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        dash->log(ERROR_LVL, "setsockopt() error: SO_REUSEADDR failed on master socket");
        throw std::runtime_error("setsockopt error: SO_REUSEADDR failed on master socket");
	}

    // Set master socket to non-blocking
	// _serverFd = fd to change permissions of, F_GETFL = gets the flags to be saved, so they're not overriden at the later if
	int flags = fcntl(_serverFd, F_GETFL, 0);
	if (flags < 0) {
        dash->log(ERROR_LVL, "fcntl() error: F_GETFL failed on master socket");
    	throw std::runtime_error("fcntl error: F_GETFL failed on master socket");
	}

	// F_SETFL = tells to change / override the fd flags with the next args
	// O_NONBLOCK = configure to open fd in non blocking, so cmd like accept or send won't block program running
	if (fcntl(_serverFd, F_SETFL, flags | O_NONBLOCK) < 0) {
        dash->log(ERROR_LVL, "fcntl() error: O_NONBLOCK failed on master socket");
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
    if (bind(_serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        dash->log(ERROR_LVL, "bind() error: unable to bind the server to a port");
        throw std::runtime_error("Bind error: unable to bind the server to a port");
    }

	// Listen takes to args, the socket that receive packets, and the size of the queue 
    // SOMAXCONN means we give maximum size allowed by the OS for the queue
    if (listen(_serverFd, SOMAXCONN) < 0) {
        dash->log(ERROR_LVL, "listen() error : unable to set the server to liste");
        throw std::runtime_error("Listen error : unable to set the server to listen");
    }

    // Create epoll instance, and instance is a monitoring tool that warns if there's
	// a change in the monitored socket (more efficient memory-wise that multi-threading)
	// The parameter is a flag, 0 means standart behavior for an epoll instance
    _epollFd = epoll_create1(0);
    if (_epollFd < 0) {
        dash->log(ERROR_LVL, "Epoll_create1() error : failed to create epoll instance");
        throw std::runtime_error("Epoll_create1 error : failed to create epoll instance");
    }

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
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &ev) < 0) {
        dash->log(ERROR_LVL, "epoll_ctl error : failed to add master socket to wacthlist");
        throw std::runtime_error("epoll_ctl error : failed to add master socket to watchlist");
    }

    dash->log(SUCCESS, "'ircserv' server successfully launched on port " + toStr(_port));

    // === Server informations for Dashboard ===
    t_column serverInfo;
    serverInfo.infoList.push_back(std::make_pair("State", "UP"));
    serverInfo.infoList.push_back(std::make_pair("Port", toStr(_port)));
    serverInfo.infoList.push_back(std::make_pair("Server fd", toStr(_serverFd)));
    serverInfo.infoList.push_back(std::make_pair("Epoll fd", toStr(_epollFd)));

    t_section server;
    server.title = "server";
    server.mainInfo = std::make_pair("mode", "Real");
    server.leftColumn = serverInfo;
    
    t_column userInfo;
    userInfo.infoList.push_back(std::make_pair("Channel created", "0"));
    userInfo.infoList.push_back(std::make_pair("Total User", "0"));
    userInfo.infoList.push_back(std::make_pair("Non Auth. User", "0"));
    userInfo.infoList.push_back(std::make_pair("Auth. User", "0"));

    t_section user;
    user.title = "user";
    user.leftColumn = userInfo;

    dash->addSection(server);
    dash->addSection(user);
    dash->render();
}

void Server::startLoop() {
    struct epoll_event events[MAX_EVENTS];

    loop {
        // Check if SIGINT have been received to shutdown properly the Server
        if (g_stop == SIGINT) {
            dash->log(INFO, "Shutdown signal received, stopping server");
            break ;
        }
        
        // Pause the socket until an event happens
		// _epollFd = the socket to watch over, events = An array of struct epoll_event
		// MAX_EVENTS = the size of the events array
		// -1 = it's the timeout arg, -1 means the socket is under sleep indefinitely until an event happen
        int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
        if (numEvents < 0) {
            if (errno == EINTR) {
                // Interrupted by a signal, check if signal is SIGINT
                continue ;
            }
            dash->log(ERROR_LVL, "epoll_wait() error: unable to pause the socket");
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

                    // Resolve and store where this client is actually connecting
					// from (reverse-DNS via NetworkUtils, falling back to the raw
					// IP) so we have a real audit trail for bans/rate-limits/logs.
                    User *newUser = getUserById(clientFd);
                    if (newUser)
                        newUser->setHostname(NetworkUtils::getHostname(clientAddr));
                    getUserById(clientFd)->setAuthenticated(true);

                    dash->log(INFO, "New client connected on fd : " + toStr(clientFd));

                    // update dashboard
                    dash->increaseInfo(dash->getSectionByIndex(1), LEFT, 1);
                    dash->increaseInfo(dash->getSectionByIndex(1), LEFT, 2);
                }
            }
            // Case B / C: an already-connected client fd fired. EPOLLOUT and
			// EPOLLIN are checked independently (not else-if) because epoll
			// can report both bits set on the same fd in the same pass.
            else {
            // Case B: the client's socket became writable again (EPOLLOUT).
			// This fires only while data is queued in the user's NetworkBuffer
			// (see Server::queueWrite/setEpollWriteInterest); flush what we can.
            if (events[i].events & EPOLLOUT) {
                flushWrite(currentFd);
            }

            // Case C: events[i].events & EPOLLIN means there are incoming
			// bytes waiting from currentFd. We therefore create a buffer in
			// which we'll receive all the incoming bytes
            if (events[i].events & EPOLLIN) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);
                
                int bytesRead = recv(currentFd, buffer, BUFFER_SIZE - 1, 0);

				// if <= 0, it means the network disconnected during the process
                if (bytesRead <= 0) {
                    dash->log(INFO, "Client disconnected on fd : " + toStr(currentFd));
                    removeUser(currentFd, "Client disconnected");
                } else {
					std::map<int, User>::iterator currentUser = _users.find(currentFd);
					if (currentUser == _users.end()) {
                        dash->log(WARNING, "Error : User not found for fd : " + toStr(currentFd));
                        continue ;
                    }
					
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
                        
                        dash->log(CLIENT, command);

        				if (!command.empty()) {
							dispatchCommand(*this, currentUser->second, command);
						}
            		}
        		}
    		}
			}
		}

		// Clean up any connection whose send() hit a fatal error (EPIPE,
		// ECONNRESET, ...) during this batch of events. Deferred to here,
		// once we're safely out of every _users/_channels iteration, so
		// removeUser()'s erase from _users can't invalidate a live loop.
		processPendingRemovals();
	}
}

void Server::stop() {
    // Clean up connections
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it) {
        close(it->first);
    }
    _users.clear();
    _channels.clear();

    if (_serverFd != -1) close(_serverFd);
    if (_epollFd != -1) close(_epollFd);
}
