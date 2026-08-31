/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHelper.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 09:41:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 09:57:14 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <algorithm>

// Adds or removes EPOLLOUT from the set of events epoll watches for this fd,
// without disturbing EPOLLIN (a client's incoming data must always be watched).
void Server::setEpollWriteInterest(int fd, bool enable) {
	struct epoll_event ev;

	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	ev.events = enable ? (EPOLLIN | EPOLLOUT) : EPOLLIN;
	epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev);
}

// EAGAIN/EWOULDBLOCK/EINTR just mean "not right now" on a non-blocking
// socket - the data is worth queuing and retrying on the next EPOLLOUT.
// Anything else (EPIPE, ECONNRESET, EBADF, ENOTCONN, ...) means the peer
// or the fd itself is gone for good: no amount of retrying will fix that.
bool Server::isFatalSendError(int err) const {
	return (err != EAGAIN && err != EWOULDBLOCK && err != EINTR);
}

// Marks clientFd for removeUser() cleanup on the next processPendingRemovals()
// pass, instead of erasing it immediately. queueWrite()/flushWrite() are
// called from inside loops over _users/_channels (broadcastServer, broadcast),
// and removeUser() erases from _users - doing that mid-iteration would
// invalidate the very iterator those loops are using.
void Server::scheduleRemoval(int fd, const std::string &reason) {
	if (std::find(_pendingRemovals.begin(), _pendingRemovals.end(), fd) != _pendingRemovals.end())
		return; // already scheduled, avoid a double removeUser() on the same fd
	
	dash->log(INFO, "Fd " + toStr(fd) + " scheduled for removal: " + reason);
	_pendingRemovals.push_back(fd);
}

// Drains _pendingRemovals, calling the exact same cleanup path as a normal
// disconnect (removeUser()). Using a pop-based loop (rather than an iterator)
// so that if removeUser() itself schedules further removals - e.g. because
// notifying a channel about this departure fails on another dead fd - those
// get processed too, safely, in the same pass.
void Server::processPendingRemovals() {
	while (!_pendingRemovals.empty()) {
		int fd = _pendingRemovals.back();
		_pendingRemovals.pop_back();

		// Might already be gone (e.g. a normal recv()==0 disconnect handled
		// it earlier in the same event batch) - nothing left to clean up.
		if (getUserById(fd))
			removeUser(fd, "Write error: connection lost");
	}
}

// Sends data to a user through their NetworkBuffer instead of a raw send() call.
// Non-blocking sockets can legitimately accept only part of the data (or none at
// all) under load; whatever the kernel didn't take is queued and EPOLLOUT is
// armed so Server::flushWrite() finishes the delivery once the socket is ready.
void Server::queueWrite(User &user, const std::string &data) {
	NetworkBuffer &buffer = user.getNetworkBuffer();

	// Data is already queued behind an earlier send: keep ordering by
	// appending, and let the pending EPOLLOUT flush drain everything in order.
	if (buffer.hasPendingWrite()) {
		buffer.queueWriteData(data);
		return;
	}

	errno = 0;
	ssize_t sent = send(user.getFd(), data.c_str(), data.size(), MSG_NOSIGNAL);

	if (sent < 0) {
		int err = errno;
		dash->log(ERROR_LVL, "queueWrite(): send crashed");
		if (isFatalSendError(err)) {
			// The connection is dead: no point queuing data for it anymore.
			scheduleRemoval(user.getFd(), "Write error: connection lost");
			return;
		}
		// Transient (EAGAIN/EWOULDBLOCK/EINTR): queue the whole message and
		// wait for the socket to become writable.
		buffer.queueWriteData(data);
		setEpollWriteInterest(user.getFd(), true);
		return;
	}

	if (static_cast<size_t>(sent) < data.size()) {
		// Partial write: queue the remainder and watch for write-readiness.
		buffer.queueWriteData(data.substr(sent));
		setEpollWriteInterest(user.getFd(), true);
	}
}

// Drains as much of a user's pending outgoing buffer as the kernel will
// currently accept. Called when epoll reports the fd as write-ready.
void Server::flushWrite(int fd) {
	User *user = getUserById(fd);
	if (!user)
		return;

	NetworkBuffer &buffer = user->getNetworkBuffer();
	const std::string &pending = buffer.getWriteBuffer();
	if (pending.empty()) {
		setEpollWriteInterest(fd, false);
		return;
	}

	errno = 0;
	ssize_t sent = send(fd, pending.c_str(), pending.size(), MSG_NOSIGNAL);
	if (sent < 0) {
		int err = errno;
		dash->log(ERROR_LVL, "flushWrite(): send crashed");
		if (isFatalSendError(err))
			scheduleRemoval(fd, "Write error: connection lost");
		return; // stay armed for the next EPOLLOUT, data remains queued
	}

	buffer.consumeWriteData(static_cast<size_t>(sent));
	if (!buffer.hasPendingWrite())
		setEpollWriteInterest(fd, false);
}

void Server::addUnauthenticatedUser(int clientFd) {
    _users.insert(std::make_pair(clientFd, User(clientFd)));
}

void Server::removeUser(int clientFd, std::string message) {
	// Get user
	User* user = getUserById(clientFd);
	if (!user) {
		dash->log(WARNING, "Unknow Fd : " + toStr(clientFd));
		return ;
	}
	
	// Get joined channels
	std::vector<std::string> channelsList = user->getJoinedChannels();
	for (std::vector<std::string>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		Channel *channel = getChannelByName(*it);
		if (!channel) {
			dash->log(WARNING, "User trying too leave a channel that doesn't exist");
			return ;
		}
		
		// Announce that user leave the server/channel
		broadcast(*channel, message);

		// Remove user from channel
		channel->removeMember(user);
		if (channel->isOperator(clientFd))
			channel->removeOperator(user);

		// If channel is empty, delete it
		if (channel->getMembers().empty()) {
			std::string message = channel->getName() + " has been deleted." + "\r\n";
			broadcastServer(message);
			_channels.erase(channel->getName());
		}

	}
	
	// Warn client that the user has been disconnected
	notification(user, "Error: closing link: " + message);
	
	// Update dashboard
	dash->decreaseInfo(dash->getSectionByIndex(1), LEFT, 1); // Decrease total of user
	if (getUserById(clientFd)->isAuthenticated())
		dash->decreaseInfo(dash->getSectionByIndex(1), LEFT, 3); // Decrease total of authenticated user
	else
		dash->decreaseInfo(dash->getSectionByIndex(1), LEFT, 2); // Decrease total of non authenticated user

	// Remove fd/user from server
    epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL);
    close(clientFd);
    _users.erase(clientFd);

}

User* Server::getUserById(int fd) {
    std::map<int, User>::iterator it = _users.find(fd);
    if (it != _users.end()) {
        return (&(it->second));
    }
    return (NULL);
}

Channel* Server::getChannelByName(const std::string &name){
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->second.getName() == name) {
            return (&(it->second));
        }
    }
    return (NULL);
}
 
User* Server::getUserByNickname(const std::string& nickname) {
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            return (&(it->second));
        }
    }
    return (NULL);
}

