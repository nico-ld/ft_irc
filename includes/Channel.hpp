#pragma once

#include <string>
#include "client.hpp"
#include <list>

struct mode {
	bool i; //for invitations
	std::list<Client> invitationList;
	std::string topic;
	std::string key;
	std::list<Client> operatorList; //o
	int l; //for limits of members
};

class Channel {
	public:
		Channel(std::string name);
		Channel(std::string name, std::string key);
		~Channel();

		void addClient(const Client &client);
		void removeClient(const Client &client);
		
		//SETTERS
		void setOperator(const Client &client);	
		
		//GETTERS
		bool isClientOperator(const Client &client, const Channel &channel);
		std::string getKey();

	private:
		std::string			_name;
		mode				_modeChannel;
		std::list<Client>	_clientList;
};
