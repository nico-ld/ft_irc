/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkBuffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:46:48 by jdessoli          #+#    #+#             */
/*   Updated: 2026/08/31 09:58:46 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetworkBuffer.hpp"

// Set up a storage place for user's incoming and outgoing text
NetworkBuffer::NetworkBuffer() : _readBuffer(""), _writeBuffer("") {}
NetworkBuffer::~NetworkBuffer() {}

// Buffer that adds bytes of data until a complete sentence is received
void NetworkBuffer::appendReadBuffer(const char* data, size_t length)
{
	if (data && length > 0)
		_readBuffer.append(data, length);
}

// Scans stored incoming text to pull out one complete, fully formed instruction.
// Chat commands are finished with line breaks (like pressing "Enter" on a keyboard).
// This function checks if a full sentence has arrived, extracts it, and removes it from storage.
// It returns true if a valid sentence was found, or false if it is still waiting for more text.
bool NetworkBuffer::extractLine(std::string& line)
{
	size_t pos = _readBuffer.find('\n');

	if (pos == std::string::npos)
		return (false);

	// Extract everything up to the newline character
	line = _readBuffer.substr(0, pos);

	// Clean up trailing carriage returns ('\r') if present
	if (!line.empty() && line[line.size() - 1] == '\r')
	{
		line.erase(line.size() - 1);
	}

	// Remove the extracted line and newline character from the read buffer
	_readBuffer.erase(0, pos + 1);
	return (true);
}

// Places outgoing server messages into a waiting line to be sent to the client.
// If the server tries to speak to a user too fast, the connection can get overwhelmed.
// This function queues up outgoing text safely so it can be delivered piece by piece.
// It prevents the server from freezing or dropping messages during heavy traffic.
void NetworkBuffer::queueWriteData(const std::string& data)
{
	_writeBuffer.append(data);
}

// Provides a look at the current queue of outgoing messages waiting to be delivered.
// The network supervisor needs to see what text is waiting before attempting to send it.
// This function grants read-only access to the outgoing text without modifying it.
// It allows the sending system to know exactly what bytes to process next.
const std::string& NetworkBuffer::getWriteBuffer() const
{
	return (_writeBuffer);
}

// Removes a specific number of bytes from the outgoing queue after successful delivery.
// Once part of a message travels through the internet, it no longer needs to be stored.
// This function trims away the front portion of the message buffer that was just delivered.
// It ensures the server never resends the same piece of text twice.
void NetworkBuffer::consumeWriteData(size_t bytesSent)
{
	if (bytesSent >= _writeBuffer.size())
	{
		_writeBuffer.clear();
	}
	else
	{
		_writeBuffer.erase(0, bytesSent);
	}
}

// Checks whether there is still unsent text remaining in the client's outgoing queue.
// The server needs to know if a connection still has pending work to deliver.
// This function returns true if there are messages waiting, and false if everything was sent.
// It helps the event monitor know when to pause or resume watching a user's connection.
bool NetworkBuffer::hasPendingWrite() const
{
	return (!_writeBuffer.empty());
}
