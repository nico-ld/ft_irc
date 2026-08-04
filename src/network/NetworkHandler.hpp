/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:13:23 by jdessoli          #+#    #+#             */
/*   Updated: 2026/08/04 20:14:24 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <sys/epoll.h>
# include <string>
# include "NetworkBuffer.hpp"

class NetworkHandler
{
	private:
		NetworkHandler();
		~NetworkHandler();

	public:
		// Modify epoll event flags to enable or disable listening for ready-to-write events (EPOLLOUT).
		static bool setWriteInterest(int epollFd, int clientFd, bool enableWrite);

		// Attempt to transmit queued outgoing data from NetworkBuffer over a non-blocking socket.
		static bool flushWriteBuffer(int clientFd, NetworkBuffer& buffer);

		// Check if an epoll event indicates an error or unexpected client disconnect.
		static bool isErrorEvent(uint32_t events);
};
