#include "Bot.hpp"

/*
 * ::processMessage() -> get in parameter a message from user,
 *						 tokenize it, identify type and then reply.
 */
int Bot::processMessage(std::string &message) {
	if (message.empty())
		return (1);

	tokenizeMessage(message);

	// Check for the presence of 'rouxbot', if not the method end here
	std::vector<std::pair<std::string, e_intent> >::iterator it;
	size_t oldSize = _tokens.size();
	for (it = _tokens.begin(); it != _tokens.end(); it++) {
		if ((*it).first == "rouxbot") {
			_tokens.erase(it);
			break ;
		}
	}
	if (oldSize == _tokens.size())
		return (1);
	
	// System output
	std::cout << ROUXBOT DIM " received : " << _tokens << RESET << std::endl;

	int scores[INTENT_UNKNOW] = {0};
	int best = INTENT_UNKNOW;
	for (it = _tokens.begin(); it != _tokens.end(); it++) {
		scores[it->second] += 1;
		if (scores[best] < scores[it->second])
			best = it->second;
	}
	_messageType = static_cast<e_intent>(best);

	std::cout << DIM "Message type is " << _messageType << RESET << std::endl;

	// Check for command in the message to execute them
	executeCommand();

	return (0);
}

/*
 * ::tokenizeMessage() -> tokenize the message into words then normalize 
 * 						  them (lowercase and remove punctuation). After 
 * 						  that, it also identify the token  type to store it.
 */
void Bot::tokenizeMessage(std::string message) {
	_tokens.clear();

	std::string word;
	std::stringstream ss(message);
	std::map<std::string, e_intent>::iterator itVoc;
	std::map<std::string, void (Bot::*)(std::string)>::iterator itAction;

	while (ss >> word) {
		// Lowrecase
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);

		// Remove punctuation (not '!' in case of command)
		for (size_t i = 0; i < word.size(); i++) {
			if (std::ispunct(word[i])) {
				if (word[i] == '!' && i == 0) {
					itAction = _actionUser.find(word);
					if (itAction == _actionUser.end())
						word.erase(i, 1);
				}
				else
					word.erase(i, 1);
			}
		}

		// Search the word inside _vocabulary to intentify it
		itVoc = _vocabulary.find(word);
		if (itVoc != _vocabulary.end())
			_tokens.push_back(std::make_pair(word, itVoc->second));
		else
			_tokens.push_back(std::make_pair(word, INTENT_UNKNOW));
	}
}

/*
 * isNameValid() -> Avoid command as User name
 */
static bool isNameValid(std::string name, e_intent type) {
	if (type == INTENT_ACTION) {
		std::cerr << DIM "Target error : " << name << " is a command." RESET << std::endl;
		return (false);
	}
	else
		return (true);
}

/*
 * ::executeCommand() -> Check for 'INTENT_ACTION' and execute command
 */
void Bot::executeCommand(void) {
	if (_tokens.empty())
		return ;

	std::vector<std::pair<std::string, e_intent> >::iterator it;
	std::map<std::string, void (Bot::*)(std::string)>::iterator itAction;
	for (it = _tokens.begin(); it != _tokens.end(); it++) {
		if (it->second == INTENT_ACTION && (it + 1) == _tokens.end()) {
			std::cerr << DIM "Target error : no target given." RESET << std::endl;
			break ;
		}
		if (it->second == INTENT_ACTION && isNameValid((it + 1)->first, (it + 1)->second)) {
			itAction = _actionUser.find(it->first);
			if (itAction != _actionUser.end()) {
				(this->*(itAction->second))((it + 1)->first);
			}
		}
	}
}
