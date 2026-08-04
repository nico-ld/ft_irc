/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkBuffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdessoli <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:46:48 by jdessoli          #+#    #+#             */
/*   Updated: 2026/08/04 20:16:19 by jdessoli         ###   ########.fr       */
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

// Scans stored text for newline breaks and extracts one full completed instruction.
// Removes the extracted command from storage and returns true if a line was found.
bool NetworkBuffer::extractLine(std::string& line)
{
	size_t pos = _readBuffer.find('\n');

	if (pos == std::string::npos)
		return false;

	// Extract everything up to the newline character
	line = _readBuffer.substr(0, pos);

	// Clean up trailing carriage returns ('\r') if present
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);

	// Remove the extracted line and newline character from the read buffer
	_readBuffer.erase(0, pos + 1);
	return true;
}

// Places outgoing server messages into a queue to be sent to the user safely.
// Prevents server freezes by storing response text until the network is ready.
void NetworkBuffer::queueWriteData(const std::string& data)
{
	_writeBuffer.append(data);
}

// Grants read-only access to inspect the outgoing text queue waiting to be sent.
// Lets the system check exactly which pending bytes need to be transmitted next.
const std::string& NetworkBuffer::getWriteBuffer() const
{
	return _writeBuffer;
}

// Erases successfully delivered bytes from the front of the outgoing message queue.
// Prevents the server from sending duplicate data after a successful network transmission.
void NetworkBuffer::consumeWriteData(size_t bytesSent)
{
	if (bytesSent >= _writeBuffer.size())
		_writeBuffer.clear();
	else
		_writeBuffer.erase(0, bytesSent);
}

// Checks if any unsent server responses are still waiting in the outgoing queue.
// Returns true if unsent text remains, helping the system track pending work.
bool NetworkBuffer::hasPendingWrite() const
{
	return !_writeBuffer.empty();
}
