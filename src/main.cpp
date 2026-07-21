#include "ft_irc.hpp"

int main(void) {
	std::string line;
	std::cout << "input > ";
	std::getline(std::cin, line);

	Parser::parse(line);
	std::cout << "Line parsed : " << std::endl;
	std::cout << "\t- Prefix : " << Parser::getPrefix() << std::endl;
	std::cout << "\t- Command : " << Parser::getCommand() << std::endl;
	
	std::vector<std::string> content = Parser::getParameters();
	std::cout << "\t- Parameters : " << content << std::endl;
	content = Parser::getTrailing();
	std::cout << "\t- Trailing : " << content << std::endl;
}
