#include "Bot.hpp"

void Bot::answerMessage(void) {
	std::cout << BOLD ORANGE "Rouxbot" RESET " : ";

	switch (_messageType)
	{
	case INTENT_NAME:
		std::cout << "Yes, that's me !";
		break;
	case INTENT_GREETING:
		std::cout << "Hi " << _userName << " !";
		break;
	case INTENT_FAREWELL:
		std::cout << "See you again !";
		break;
	case INTENT_THANKS:
		std::cout << "No problems !";
		break;
	case INTENT_QUESTION:
		std::cout << "For timing reasons, I'm not enought smart to answer this..";
		break;
	case INTENT_INSULT:
		std::cout << "Miror ! >:(";
		break;
	case INTENT_ACTION:
		std::cout << "For sure, I'm on it !";
		break;
	default:
		std::cout << "I don't really understand what you say.. Sorry about that.. :(";
		break;
	}
	std::cout << std::endl;
}