#include "Channel.hpp"

Channel::Channel(std::string name) {
	_name = name;
	_modeChannel.i = false;
	_modeChannel.l = 0;
}

Channel::Channel(std::string name, std::string key) {
	_name = name;
	_modeChannel.key = key;
	_modeChannel.i = false;
	_modeChannel.l = 0;
}

Channel::~Channel() {}

void Channel::addClient(const User &client) {
	_clientList.push_back(client);
}

void Channel::removeClient(const User &client) {
	std::list<User>::iterator it;
	for (it = _clientList.begin(); (*it).getName() != client.getName(); ++it) {}
	_clientList.erase(it);
}

//				SETTERS

void Channel::setOperator(const User &client) {
	_modeChannel.operatorList.push_back(client);
}


//				GETTERS

bool Channel::isClientOperator(const User &client, const Channel &channel) {
	std::list<User>::const_iterator it = channel._modeChannel.operatorList.begin();
	while (it != channel._modeChannel.operatorList.end()) {
		if ((*it).getName() == client.getName())
			return true;
	}
	return false;
}

std::string Channel::getKey() {
	return _modeChannel.key;
}
