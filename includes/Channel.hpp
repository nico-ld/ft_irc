#pragma once

#include <string>
#include "user.hpp"
#include <list>

struct mode {
	bool i; //for invitations
	std::list<User> invitationList;
	std::string topic;
	std::string key;
	std::list<User> operatorList; //o
	int l; //for limits of members
};

class Channel {
	public:
		Channel(std::string name);
		Channel(std::string name, std::string key);
		~Channel();

		void addClient(const User &client);
		void removeClient(const User &client);
		
		//SETTERS
		void setOperator(const User &client);	
		
		//GETTERS
		bool isClientOperator(const User &client, const Channel &channel);
		std::string Channel::getKey();

	private:
		std::string			_name;
		mode				_modeChannel;
		std::list<User>	_clientList;
};
