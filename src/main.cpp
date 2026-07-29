#include "ft_irc.hpp"
#include <cstdlib>

int main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "Error: " 
					<< ((ac < 3) ? "not enought" : "too many") 
					<< " arguments." 
					<< std::endl;
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	char	*ptrEnd;
	int		port = (int)strtod(av[1], &ptrEnd);
	if (ptrEnd) {
		std::cerr << "Error: invalid value for server port." << std::endl;
		return (1);
	}

	Server server(port, av[2]);
	server.init();
	server.startLoop();
}