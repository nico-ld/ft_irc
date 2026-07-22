#pragma once

#include "../../includes/ft_irc.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <utility>

// === Text decoration ===
#define RESET "\033[0m"

# define ORANGE	"\033[38;2;255;153;0m"

#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"

// === RouxBot label ===
#define ROUXBOT DIM ITALIC "RouxBot" RESET

enum e_intent {
	INTENT_NAME,
	INTENT_GREETING,
	INTENT_FAREWELL,
	INTENT_THANKS,
	INTENT_QUESTION,
	INTENT_INSULT,
	INTENT_ACTION,
	INTENT_UNKNOW
};

class Bot
{
	public:
		// === Constructor & Destructor === 
		Bot();
		~Bot();

		// === Methods ===
		// ~~ Messages methods ~~
		int processMessage(std::string &message);

		// ~~ Users methods ~~
		void setUser(std::string userName);
		void setUserBehavior(std::string userName);
		void addUser(std::string userName);
		void deleteUser(std::string userName);
		std::string getUserInfo(std::string userName) const;
		
		// === Exceptions ===
		class CannotOpenFileException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidJsonFormatException: public std::exception {
			public:
				virtual const char* what() const throw();
		};

	private:
		// === User ===
		std::string		_userName;
		std::string		_usersFileName;
		int				_userBehavior;

		// === Message ===
		e_intent _messageType;
		std::vector<std::pair<std::string, e_intent> > _tokens;

		// === Vocabulary ===
		void _initVocabulary( void );
		std::map<std::string, e_intent> _vocabulary;

		// ~~ User action ~~
		std::map<std::string, void (Bot::*)(std::string)> _actionUser;
		void _initActionUser( void );

		// ~~ Methods ~~
		void _initPoliteness( void );
		void _initQuestion ( void );
		void _initInsult ( void );

		// === Private Methods ===
		// ~~ Messages methods ~~
		void tokenizeMessage(std::string message);
		void executeCommand( void );
		void answerMessage( void );
};

std::ostream &operator<<(std::ostream &out, std::vector<std::pair<std::string, e_intent> > content);
std::ostream &operator<<(std::ostream &out, e_intent &type);