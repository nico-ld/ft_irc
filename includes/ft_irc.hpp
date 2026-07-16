/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 08:36:12 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/16 08:46:31 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define RESET		"\033[0m"

// > Styles
#define BOLD		"\033[1m"
#define DIM			"\033[2m"
#define ITALIC		"\033[3m"
#define UNDERLINE	"\033[4m"
#define BLINK		"\033[5m"
#define REVERSE		"\033[7m"
#define HIDDEN		"\033[8m"
#define STRIKE		"\033[9m"

// > Colors
#define ORANGE		"\033[38;2;255;153;0m"
#define PURPLE		"\033[38;2;153;51;255m"
#define RED			"\033[91m"
#define GREEN		"\033[92m"
#define YELLOW		"\033[93m"
#define BLUE		"\033[94m"
#define MAGENTA		"\033[95m"
#define CYAN		"\033[96m"
#define WHITE		"\033[97m"

// > Includes
#include "Parser.hpp"
