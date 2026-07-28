/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:19:26 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/28 16:20:07 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <netinet/in.h>

class NetworkUtils
{
	private:
		// Private constructor: This class is a collection of static utility functions.
		// It should not be instantiated.
		NetworkUtils();
		~NetworkUtils();

	public:
		// Convert a raw binary IPv4 address into a human-readable IP string (e.g., "192.168.1.1").
		static std::string getIpString(const struct sockaddr_in& addr);

		// Perform a reverse DNS look-up to find the domain/hostname associated with an address.
		// Falls back to returning the IP string if no hostname can be found.
		static std::string getHostname(const struct sockaddr_in& addr);
};
