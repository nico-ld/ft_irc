#include "Bot.hpp"

/*
 * updateUserfile() -> overwrite users.json with new content
 */
static int updateUserFile(std::vector<std::string> newContent, std::string fileName) {
	std::fstream userFile(fileName.c_str(), std::ios::out | std::ios::trunc);
	if (!userFile.is_open())
		return (-1);

	std::vector<std::string>::iterator it;
	for (it = newContent.begin(); it != newContent.end(); it++)
		userFile << *it;
	userFile.close();
	return (0);
}

/*
 * ::setUser() -> define wich user is talking to RouxBot
 */
void Bot::setUser(std::string userName) {
	if (userName.empty())
		return ;

	_userName = userName;
	
	std::string userInfo = getUserInfo(userName);
	if (userInfo.empty()) {
		addUser(userName);
		_userBehavior = 0;
	}
	else {
		size_t pos = userInfo.find("behavior");
		if (pos == std::string::npos)
			throw InvalidJsonFormatException();
		pos += 10;
		
		_userBehavior = atoi(userInfo.c_str() + pos);
	}

	// System output
	std::cout << DIM "User setted on " << _userName << ", behavior : " << _userBehavior << RESET << std::endl;
}

/*
 * ::setUserBehavior() -> Modify behavior of a user
 */
void Bot::setUserBehavior(std::string userName) {
	if (userName.empty())
		return ;

	std::string input;
	std::cout << "Enter the new Behavior value [-10, 10] (STOP to quit)\n > ";
	std::getline(std::cin, input);
	if (input == "STOP")
		return ;

	// Check for valid input
	char *end;
	int n = std::strtol(input.c_str(), &end, 10);
	while ((n == 0 && *end) || n < -10 || n > 10) {
		std::cerr << "Error:  please enter an integer between -10 and 10 (STOP to quit)\n > ";
		std::getline(std::cin, input);
		n = std::strtol(input.c_str(), &end, 10);
		if (input == "STOP")
			return ;
	}

	std::ifstream userFile(_usersFileName.c_str());
	if (!userFile.is_open())
		throw CannotOpenFileException();
	
	// Get users.json to modify the right line
	std::vector<std::string> newContent;
	std::string line;
	while (getline(userFile, line)) {
		if (line.find(userName, 0) == std::string::npos)
			newContent.push_back(line.append("\n"));
		else {
			size_t pos = line.find("behavior");
			std::stringstream ss;
			ss << n;
			std::string newBehavior = ss.str();
			line.replace(pos + 10, newBehavior.size(), newBehavior);
			newContent.push_back(line.append("\n"));
		}
	}
	userFile.close();

	// Update users.json
	if (updateUserFile(newContent, _usersFileName) == -1)
		throw CannotOpenFileException();
	std::cout << DIM << userName << " behavior updated to " << n << RESET << std::endl;
}

/*
 * ::addUser() -> add the target user if hee doesn't exist
 */
void Bot::addUser(std::string userName) {
	if (userName.empty())
		return ;

	std::string userInfo = getUserInfo(userName);
	if (!userInfo.empty())
		return ;

	std::ofstream usersFile(_usersFileName.c_str(), std::ios::app);
	if (!usersFile.is_open())
		throw CannotOpenFileException();
	
	usersFile << "{\"name\":\"" << userName << "\", \"behavior\":0}" << std::endl;
	usersFile.close();
	std::cout << DIM "User : " ITALIC << userName << RESET DIM " added." RESET << std::endl;
}

/*
 * ::deleteUser() -> delete the target user
 */
void Bot::deleteUser(std::string userName) {
	if (userName.empty())
		return ;

	std::fstream usersFile(_usersFileName.c_str());
	if (!usersFile.is_open())
		throw CannotOpenFileException();

	std::string line;
	std::vector<std::string> updatedFile;
	while (getline(usersFile, line)) {
		if (line.find(userName, 0) == std::string::npos) {
			updatedFile.push_back(line.append("\n"));
		}
	}
	usersFile.close();

	if (updateUserFile(updatedFile, _usersFileName) == -1)
		throw CannotOpenFileException();
	std::cout << DIM "User : " ITALIC << userName << RESET DIM " have been deleted" RESET << std::endl;
}

/*
 * ::getUserInfo() -> return users info from the users.json
 */
std::string Bot::getUserInfo(std::string userName) const {
	if (userName.empty())
		return ("");

	std::fstream usersFile(_usersFileName.c_str());
	if (!usersFile.is_open()) {
		usersFile.open(_usersFileName.c_str(), std::ios::out);
		if (usersFile.is_open())
			return ("");
		throw CannotOpenFileException();
	}
	
	std::string line;
	while (std::getline(usersFile, line)) {
		if (line.find(userName, 0) != std::string::npos) {
			usersFile.close();
			return (line);
		}
	}
	usersFile.close();
	return ("");
}
