#pragma once

#include "../../includes/ft_irc.hpp"

// === Text decoration ===
#define RESET "\033[0m"

# define ORANGE	"\033[38;2;255;153;0m"

#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"

// === Text patern ===
#define ROUXBOT DIM ITALIC "RouxBot" RESET
#define ERROR  RED BOLD "ERROR: " RESET

/* > Create a user for the bot on the server, return 1 on error */
int	registerBot(int sock, std::string password);
