#include "Bot.hpp"

// === Constructor & Destructor ===
Bot::Bot() {
	_usersFileName = "users.json";
	_initVocabulary();
	
	std::cout << ROUXBOT DIM " is ready." RESET << std::endl;
}

Bot::~Bot() {
	std::cout << ROUXBOT DIM " is down." RESET << std::endl;
}

// === Exceptions ===
const char* Bot::CannotOpenFileException::what() const throw() {
	return ("Error: Cannot open 'users.json'");
}

const char* Bot::InvalidJsonFormatException::what() const throw() {
	return ("Error: `users.json` invalid format.");
}

std::ostream &operator<<(std::ostream &out, std::vector<std::pair<std::string, e_intent> > content) {
	std::vector<std::pair<std::string, e_intent> >::iterator it;

	for (it = content.begin(); it != content.end(); it++) {
		out << it->first << " ";
	}
	return (out);
}

std::ostream &operator<<(std::ostream &out, e_intent &type) {
	switch (type)
	{
	case INTENT_NAME:
		out << "NAME";
		break;
	case INTENT_GREETING:
		out << "GREETING";
		break;
	case INTENT_FAREWELL:
		out << "FAREWELL";
		break;
	case INTENT_THANKS:
		out << "THANKS";
		break;
	case INTENT_QUESTION:
		out << "QUESTION";
		break;
	case INTENT_INSULT:
		out << "INSULT";
		break;
	case INTENT_ACTION:
		out << "ACTION";
		break;
	default:
		out << "UNKNOW";
		break;
	}

	return (out);
}
