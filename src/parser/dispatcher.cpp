/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/20 09:56:45 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

static void	channelCommandsDispatch( void ) {
	/* code */
}

static void messageCommandsDispatch( void ) {
	/* code */
}

static void userCommandsDispatch( void ) {
	/* code */
}

void dispatch( void ) {
	std::string command = Parser::getCommand();

	switch (Parser::getCommandListId())
	{
	case 1:
		channelCommandsDispatch();
		break ;
	case 2:
		messageCommandsDispatch();
		break ;
	case 3:
		userCommandsDispatch();
		break ;

	default:
		throw Parser::InvalidCommandException();
		break ;
	}
}
