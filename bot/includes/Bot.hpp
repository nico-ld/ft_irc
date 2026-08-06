#pragma once

#include "../../includes/Parser.hpp"
#include "../../includes/colors.hpp"

// === Text patern ===
#define ROUXBOT DIM ITALIC "RouxBot" RESET

// === Buffer ===
#define BUFFER_SIZE 512

/* > Create a user for the bot on the server, return 1 on error */
int	registerBot(int sock, std::string password);

/* > Start the loop that read server output */
void serverLoop(int sock);

class Bot
{
	private:
		std::string _command;
		std::vector<std::string> _parameters;

	public:
		// == Constructor and destructor ==
		Bot();
		Bot(const Bot &other);
		~Bot();

		// === OVERLOADS === 
		Bot &operator=(const Bot &other);

		// === GETTERS / SETTERS ===
		const std::string	&getCommand( void ) const;
		void				setCommand(std::string command);

		const std::vector<std::string>	&getParameters( void ) const;
		void							setParameters(std::vector<std::string> parameters);
};
