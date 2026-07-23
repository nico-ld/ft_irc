#include "Bot.hpp"



void Bot::answerMessage(void) {
	std::cout << BOLD ORANGE "Rouxbot" RESET " : ";

	switch (_messageType)
	{
	case INTENT_NAME:
		if (_badBotName)
			std::cout << "Not " << _invalidBotName << " but RouxBot.";
		else 
			std::cout << "Yes, that's me !";
		break;

	case INTENT_GREETING:
		if (_badBotName)
			std::cout << "Hi " << _userName << ". But my name is RouxBot, not " << _invalidBotName << ".";
		else
			std::cout << "Hi " << _userName << " !";
		break;

	case INTENT_FAREWELL:
		if (_badBotName)
			std::cout << "Goodbye.";
		else
			std::cout << "See you again !";
		break;

	case INTENT_THANKS:
		if (_badBotName)
			std::cout << "My name is 'RouxBot' not '" << _badBotName << "' !";
		else
			std::cout << "No problems !";
		break;

	case INTENT_QUESTION:
		if (_badBotName)
			std::cout << "Due to timing reasons I can't answer you, but I can reconized my name: 'RouxBot' not '" << _badBotName << "'.";
		else
			std::cout << "For timing reasons, I'm not enought smart to answer this..";
		break;

	case INTENT_INSULT:
		if (_badBotName)
			std::cout << ">:[";
		else
			std::cout << "Miror ! >:(";
		break;

	case INTENT_ACTION:
		if (_badBotName)
			std::cout << "I'm not paid to do that..";
		else
			std::cout << "For sure, I'm on it !";
		break;

	default:
		if (_badBotName)
			std::cout << "I don't understand, and I don't wan't to understand. My name is 'RouxBot' not '" << _invalidBotName << "'.";
		else
			std::cout << "I don't really understand what you say.. Sorry about that.. :(";
		break;
	}

	std::cout << std::flush << std::endl;
}