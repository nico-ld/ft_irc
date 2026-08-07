/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ansi.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:43:24 by nico              #+#    #+#             */
/*   Updated: 2026/08/07 14:41:11 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef ANSI_HPP
# define ANSI_HPP

#define RESET		"\033[0m"

/* === STYLES === */
#define BOLD		"\033[1m"
#define DIM			"\033[2m"
#define ITALIC		"\033[3m"
#define UNDERLINE	"\033[4m"
#define BLINK		"\033[5m"
#define REVERSE		"\033[7m"
#define HIDDEN		"\033[8m"
#define STRIKE		"\033[9m"

/* === COLORS === */
#define ORANGE		"\033[38;2;255;153;0m"
#define PURPLE		"\033[38;2;153;51;255m"
#define RED			"\033[91m"
#define GREEN		"\033[92m"
#define YELLOW		"\033[93m"
#define BLUE		"\033[94m"
#define MAGENTA		"\033[95m"
#define CYAN		"\033[96m"
#define WHITE		"\033[97m"

/* === CURSOR MOVES === */
#include <string>
#include <sstream>

// Move cursor up N lines
inline std::string moveUp(int n) {
	std::ostringstream oss;
	oss << "\033[" << n << "A";
	return (oss.str());
}

// Move cursor down N lines
inline std::string moveDown(int n) {
	std::ostringstream oss;
	oss << "\033[" << n << "B";
	return (oss.str());
}

// Move cursor right N columns
inline std::string moveRight(int n) {
	std::ostringstream oss;
	oss << "\033[" << n << "C";
	return (oss.str());
}

// Move cursor left N columns
inline std::string moveLeft(int n) {
	std::ostringstream oss;
	oss << "\033[" << n << "D";
	return (oss.str());
}

// Set cursor to position X (column) and Y (row)
inline std::string setCursor(int row, int column) {
	std::ostringstream oss;
	oss << "\033[" << row << ";" << column << "H";
	return (oss.str());
}

// Save the current position of cursor
#define SAVE_CURSOR "\033[s"

// Restore the saved cursor position
#define RESTORE_CURSOR "\033[u"

/* === ERASING CONTENT === */
#define SCREEN_CLEAR "\033[2J"
#define RIGHT_CLEAR "\033[0K"
#define LEFT_CLEAR "\033[1K"
#define LINE_CLEAR "\033[2K"

/* === PATERNS === */
#define ERROR RED BOLD "ERROR: " RESET
#define ROUXBOT BOLD ORANGE "Roux" PURPLE "Bot" RESET

#define INFO CYAN "INFO" RESET
#define DEBUG YELLOW "DEBUG" RESET
#define ERROR_LVL RED "ERROR" RESET
#define SUCCESS GREEN "SUCCESS" RESET
#define SYSTEM BLUE "SYSTEM" RESET
#define SERVER DIM "SERVER" RESET

#endif
