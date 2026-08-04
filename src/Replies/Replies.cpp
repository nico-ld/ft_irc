#include "Server.hpp"
#include "User.hpp"

void Server::sendReply(User &user, const std::string &code, const std::string &rest) {
	std::string nick = user.getNickname();
	if (nick.empty())
		nick = "*";
	
	std::string msg = ":ircserv " + code + " " + nick + " " + rest + "\r\n";
	send(user.getFd(), msg.c_str(), msg.size(), 0);
}
