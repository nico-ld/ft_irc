/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:20:29 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/28 16:30:10 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetworkUtils.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

NetworkUtils::NetworkUtils() {}
NetworkUtils::~NetworkUtils() {}

// Converts a raw IP address into standard text format
// if the conversion fails, it returns a safe default string
std::string NetworkUtils::getIpString(const struct sockaddr_in& addr)
{
	char ipBuffer[INET_ADDRSTRLEN];

	std::memset(ipBuffer, 0, sizeof(ipBuffer));
	if (inet_ntop(AF_INET, &(addr.sin_addr), ipBuffer, INET_ADDRSTRLEN) != NULL)
	{
		return (std::string(ipBuffer));
	}
	return ("127.0.0.1");
}

// Finds an ip through its host name
// if it finds the host name, it returns it, otherwise it returns the ip adress
std::string NetworkUtils::getHostname(const struct sockaddr_in& addr)
{
	char hostBuffer[NI_MAXHOST];

	std::memset(hostBuffer, 0, sizeof(hostBuffer));
	int result = getnameinfo(
		(struct sockaddr*)&addr,
		sizeof(addr),
		hostBuffer,
		sizeof(hostBuffer),
		NULL,
		0,
		0
	);

	if (result == 0 && hostBuffer[0] != '\0')
	{
		return (std::string(hostBuffer));
	}
	return (getIpString(addr));
}
