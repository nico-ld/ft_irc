#include "Bot.hpp"

void Bot::_initVocabulary(void) {
	_vocabulary["rouxbot"] = INTENT_NAME;
	_initActionUser();
	_initPoliteness();
	_initQuestion();
	_initInsult();
}

void Bot::_initActionUser(void) {
	_actionUser["!delete"] = &Bot::deleteUser;
	_actionUser["!add"] = &Bot::addUser;
	_actionUser["!behavior"] = &Bot::setUserBehavior;
	_actionUser["!set"] = &Bot::setUser;
	
	_vocabulary["!delete"] = INTENT_ACTION;
	_vocabulary["!add"] = INTENT_ACTION;
	_vocabulary["!behavior"] = INTENT_ACTION;
	_vocabulary["!set"] = INTENT_ACTION;
}

void Bot::_initPoliteness(void) {
	_vocabulary["hello"] = INTENT_GREETING;
	_vocabulary["hi"] = INTENT_GREETING;
	_vocabulary["hey"] = INTENT_GREETING;
	_vocabulary["yo"] = INTENT_GREETING;
	_vocabulary["bye"] = INTENT_FAREWELL;
	_vocabulary["goodbye"] = INTENT_FAREWELL;
	_vocabulary["thanks"] = INTENT_THANKS;
	_vocabulary["thank"] = INTENT_THANKS;
}

void Bot::_initQuestion(void) {
	_vocabulary["what"] = INTENT_QUESTION;
	_vocabulary["why"] = INTENT_QUESTION;
	_vocabulary["where"] = INTENT_QUESTION;
	_vocabulary["which"] = INTENT_QUESTION;
	_vocabulary["how"] = INTENT_QUESTION;
}

void Bot::_initInsult(void) {
	_vocabulary["idiot"] = INTENT_INSULT;
	_vocabulary["moron"] = INTENT_INSULT;
	_vocabulary["stupid"] = INTENT_INSULT;
	_vocabulary["dumb"] = INTENT_INSULT;
	_vocabulary["fool"] = INTENT_INSULT;
	_vocabulary["jerk"] = INTENT_INSULT;
	_vocabulary["weirdo"] = INTENT_INSULT;
	_vocabulary["jackass"] = INTENT_INSULT;
}
