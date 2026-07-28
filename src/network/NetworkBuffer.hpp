/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkBuffer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:38:57 by jdessoli          #+#    #+#             */
/*   Updated: 2026/07/28 17:46:01 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>

class NetworkBuffer
{
	private:
		std::string _readBuffer;
		std::string _writeBuffer;

	public:
		NetworkBuffer();
		~NetworkBuffer();

		// Add raw incoming data to the buffer
		void appendReadBuffer(const char* data, size_t length);

		// Extract one complete command from incoming data
		bool extractLine(std::string& line);

		// Add message text to the queue waiting to be sent out
		void queueWriteData(const std::string& data);

		// Check the outgoing queued data
		const std::string& getWriteBuffer() const;

		// Remove bytes from the outgoing queue that have successfully been sent
		void consumeWriteData(size_t bytesSent);

		// Check if there is still data waiting to be sent
		bool hasPendingWrite() const;
};
