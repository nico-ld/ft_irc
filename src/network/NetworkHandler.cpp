/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:14:28 by jdessoli          #+#    #+#             */
/*   Updated: 2026/08/04 20:15:39 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetworkHandler.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

NetworkHandler::NetworkHandler() {}
NetworkHandler::~NetworkHandler() {}

// Tells the operating system whether to watch for when a client socket is ready to receive data.
// Dynamically toggles EPOLLOUT on epoll so write events only trigger when pending data exists.
bool NetworkHandler::setWriteInterest(int epollFd, int clientFd, bool enableWrite)
{
	struct epoll_event ev;

	std::memset(&ev, 0, sizeof(ev));
	ev.data.fd = clientFd;
	ev.events = EPOLLIN;
	if (enableWrite)
		ev.events |= EPOLLOUT;

	if (epoll_ctl(epollFd, EPOLL_CTL_MOD, clientFd, &ev) < 0)
		return false;

	return true;
}

// Sends queued outgoing text across the network to a client without blocking the server.
// Transmits as many bytes as possible and updates the buffer with remaining unsent data.
bool NetworkHandler::flushWriteBuffer(int clientFd, NetworkBuffer& buffer)
{
	if (!buffer.hasPendingWrite())
		return true;

	const std::string& data = buffer.getWriteBuffer();
	ssize_t sent = send(clientFd, data.c_str(), data.size(), 0);

	if (sent < 0)
		return false;

	buffer.consumeWriteData(static_cast<size_t>(sent));
	return true;
}

// Inspects raw event signals from epoll to detect connection errors or sudden drops.
// Identifies EPOLLERR and EPOLLHUP flags so broken sockets can be cleanly closed.
bool NetworkHandler::isErrorEvent(uint32_t events)
{
	if ((events & EPOLLERR) || (events & EPOLLHUP))
		return true;

	return false;
}
